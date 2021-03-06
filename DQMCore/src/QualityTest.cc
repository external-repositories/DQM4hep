//==========================================================================
//  DQM4hep a data quality monitoring software for high energy physics 
//--------------------------------------------------------------------------
//
// For the licensing terms see $DQM4hep_DIR/LICENSE.
// For the list of contributors see $DQM4hep_DIR/AUTHORS.
//
// Author     : R.Ete
//====================================================================

// -- dqm4hep headers
#include <dqm4hep/Logging.h>
#include <dqm4hep/MonitorElement.h>
#include <dqm4hep/QualityTest.h>

namespace dqm4hep {

  namespace core {

    QualityTestReport::QualityTestReport(const QualityTestReport &qreport) {
      *this = qreport;
    }

    //-------------------------------------------------------------------------------------------------

    QualityTestReport &QualityTestReport::operator=(const QualityTestReport &qreport) {
      m_qualityTestName = qreport.m_qualityTestName;
      m_qualityTestType = qreport.m_qualityTestType;
      m_qualityTestDescription = qreport.m_qualityTestDescription;
      m_monitorElementName = qreport.m_monitorElementName;
      m_monitorElementType = qreport.m_monitorElementType;
      m_monitorElementPath = qreport.m_monitorElementPath;
      m_message = qreport.m_message;
      m_quality = qreport.m_quality;
      m_qualityFlag = qreport.m_qualityFlag;
      m_extraInfos = qreport.m_extraInfos;
      return *this;
    }

    //-------------------------------------------------------------------------------------------------

    void QualityTestReport::toJson(json &value) const {
      value = {{"qualityTestType", m_qualityTestType},
               {"qualityTestName", m_qualityTestName},
               {"qualityTestDescription", m_qualityTestDescription},
               {"monitorElementName", m_monitorElementName},
               {"monitorElementType", m_monitorElementType},
               {"monitorElementPath", m_monitorElementPath},
               {"message", m_message},
               {"quality", m_quality},
               {"flag", m_qualityFlag},
               {"extra", m_extraInfos}};
    }

    //-------------------------------------------------------------------------------------------------

    void QualityTestReport::fromJson(const json &value) {

      m_qualityTestType = value.value<std::string>("qualityTestType", m_qualityTestType);
      m_qualityTestName = value.value<std::string>("qualityTestName", m_qualityTestName);
      m_qualityTestDescription = value.value<std::string>("qualityTestDescription", m_qualityTestDescription);
      m_monitorElementType = value.value<std::string>("monitorElementType", m_monitorElementType);
      m_monitorElementName = value.value<std::string>("monitorElementName", m_monitorElementName);
      m_monitorElementPath = value.value<std::string>("monitorElementPath", m_monitorElementPath);
      m_message = value.value<std::string>("message", m_message);
      m_quality = value.value<float>("quality", m_quality);
      m_qualityFlag = value.value<QualityFlag>("flag", m_qualityFlag);
      m_extraInfos = value.value<json>("extra", m_extraInfos);
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    QReportStorage::QReportStorage() : m_reports() {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    void QReportStorage::addReport(const QReport &qreport, bool warnOnReplace) {
      const std::string &path(qreport.m_monitorElementPath);
      const std::string &name(qreport.m_monitorElementName);
      const std::string &qtname(qreport.m_qualityTestName);

      if (!warnOnReplace) {
        QReportContainer::key_type key(path, name);
        m_reports[key][qtname] = qreport;
      } else {
        QReportContainer::key_type key(path, name);
        auto iter1 = m_reports.find(key);

        if (m_reports.end() == iter1)
          iter1 = m_reports.insert(QReportContainer::value_type(key, QReportMap())).first;

        auto iter2 = iter1->second.find(qtname);

        if (iter1->second.end() == iter2) {
          iter2 = iter1->second.insert(QReportMap::value_type(qtname, qreport)).first;
        } else {
          dqm_warning("QReportStorage::addReport: Replacing qreport path '{0}', name '{1}', qtest '{2}'", path, name,
                      qtname);
          iter2->second = qreport;
        }
      }
    }

    //-------------------------------------------------------------------------------------------------

    void QReportStorage::addReports(const QReportMap &qreports, bool warnOnReplace) {
      for (auto iter : qreports)
        this->addReport(iter.second, warnOnReplace);
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode QReportStorage::report(const std::string &path, const std::string &name,
                                      const std::string &qualityTestName, QReport &qreport) {
      QReportContainer::key_type key(path, name);
      auto findIter = m_reports.find(key);

      if (m_reports.end() == findIter)
        return STATUS_CODE_NOT_FOUND;

      auto findIter2 = findIter->second.find(qualityTestName);

      if (findIter->second.end() == findIter2)
        return STATUS_CODE_NOT_FOUND;

      qreport = findIter2->second;
      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode QReportStorage::reports(const std::string &path, const std::string &name, QReportMap &qreports) {
      QReportContainer::key_type key(path, name);
      auto findIter = m_reports.find(key);

      if (m_reports.end() == findIter)
        return STATUS_CODE_NOT_FOUND;

      qreports.insert(findIter->second.begin(), findIter->second.end());
      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode QReportStorage::reportsQualityHigher(const std::string &path, const std::string &name, float qlimit,
                                                    QReportMap &qreports) {
      if (0.f < qlimit || qlimit > 1.f)
        return STATUS_CODE_OUT_OF_RANGE;

      QReportContainer::key_type key(path, name);
      auto findIter = m_reports.find(key);

      if (m_reports.end() == findIter)
        return STATUS_CODE_NOT_FOUND;

      for (auto qtest : findIter->second) {
        if (qtest.second.m_quality >= qlimit) {
          qreports.insert(QReportMap::value_type(qtest.first, qtest.second));
        }
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode QReportStorage::reportsQualityLower(const std::string &path, const std::string &name, float qlimit,
                                                   QReportMap &qreports) {
      if (0.f < qlimit || qlimit > 1.f)
        return STATUS_CODE_OUT_OF_RANGE;

      QReportContainer::key_type key(path, name);
      auto findIter = m_reports.find(key);

      if (m_reports.end() == findIter)
        return STATUS_CODE_NOT_FOUND;

      for (auto qtest : findIter->second) {
        if (qtest.second.m_quality <= qlimit) {
          qreports.insert(QReportMap::value_type(qtest.first, qtest.second));
        }
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    const QReportContainer &QReportStorage::reports() {
      return m_reports;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode QReportStorage::reportsQualityHigher(float qlimit, QReportContainer &qreports) {
      if (0.f < qlimit || qlimit > 1.f)
        return STATUS_CODE_OUT_OF_RANGE;

      for (auto iter1 : m_reports) {
        QReportMap reportMap;

        for (auto iter2 : iter1.second) {
          if (iter2.second.m_quality >= qlimit) {
            reportMap.insert(QReportMap::value_type(iter2.first, iter2.second));
          }
        }

        if (reportMap.empty())
          continue;

        qreports.insert(QReportContainer::value_type(iter1.first, reportMap));
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    StatusCode QReportStorage::reportsQualityLower(float qlimit, QReportContainer &qreports) {
      if (0.f < qlimit || qlimit > 1.f)
        return STATUS_CODE_OUT_OF_RANGE;

      for (auto iter1 : m_reports) {
        QReportMap reportMap;

        for (auto iter2 : iter1.second) {
          if (iter2.second.m_quality <= qlimit) {
            reportMap.insert(QReportMap::value_type(iter2.first, iter2.second));
          }
        }

        if (reportMap.empty())
          continue;

        qreports.insert(QReportContainer::value_type(iter1.first, reportMap));
      }

      return STATUS_CODE_SUCCESS;
    }

    //-------------------------------------------------------------------------------------------------

    void QReportStorage::clear() {
      m_reports.clear();
    }
    
    //-------------------------------------------------------------------------------------------------
    
    void QReportStorage::toJson(json &object) const {      
      for (const auto &iter : m_reports) {
        for (const auto &iter2 : iter.second) {
          json jsonQReport;
          iter2.second.toJson(jsonQReport);
          object.push_back(jsonQReport);
        }
      }
    }

    //-------------------------------------------------------------------------------------------------
    //-------------------------------------------------------------------------------------------------

    float QualityTest::m_defaultWarningLimit = 0.8;
    float QualityTest::m_defaultErrorLimit = 0.5;
    
    //-------------------------------------------------------------------------------------------------

    QualityTest::QualityTest(const std::string &qtype, const std::string &qname) :
      m_type(qtype),
      m_name(qname),
      m_description("") {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    QualityTest::~QualityTest() {
      /* nop */
    }

    //-------------------------------------------------------------------------------------------------

    const std::string &QualityTest::type() const {
      return m_type;
    }

    //-------------------------------------------------------------------------------------------------

    const std::string &QualityTest::name() const {
      return m_name;
    }

    //-------------------------------------------------------------------------------------------------

    const std::string &QualityTest::description() const {
      return m_description;
    }

    //-------------------------------------------------------------------------------------------------

    void QualityTest::run(MonitorElement* monitorElement, QualityTestReport &report) {
      this->fillBasicInfo(monitorElement, report);

      if (nullptr == monitorElement) {
        report.m_message = "Couldn't run quality test: monitor element pointer is nullptr";
        report.m_quality = 0.f;
        report.m_qualityFlag = INVALID;
        return;
      }

      if (nullptr == monitorElement->object()) {
        report.m_message = "Couldn't run quality test: ROOT monitor object is nullptr";
        report.m_quality = 0.f;
        report.m_qualityFlag = INVALID;
        return;
      }
      
      if(!this->enoughStatistics(monitorElement)) {
        report.m_message = "Couldn't run quality test: Not enough statistics !";
        report.m_quality = 0.f;
        report.m_qualityFlag = INSUFFICENT_STAT;
        return;
      }

      try {
        this->userRun(monitorElement, report);
        
        if(report.m_quality >= 0.f && report.m_quality < this->errorLimit()) {
          report.m_qualityFlag = ERROR;
        }
        else if(report.m_quality >= this->errorLimit() && report.m_quality < this->warningLimit()) {
          report.m_qualityFlag = WARNING;
        }
        else if(report.m_quality >= this->warningLimit() && report.m_quality <= 1.f) {
          report.m_qualityFlag = SUCCESS;
        }
        else {
          const std::string message("Quality value (" + typeToString(report.m_quality) + ") is out range !");
          
          if (!report.m_message.empty())
            report.m_message += " " + message;
          else
            report.m_message = message;
            
          throw StatusCodeException(STATUS_CODE_OUT_OF_RANGE);
        }
      } catch (StatusCodeException &exception) {
        const std::string message("Caught StatusCodeException while run QTest: " + exception.toString());

        if (!report.m_message.empty())
          report.m_message += " " + message;
        else
          report.m_message = message;

        report.m_quality = 0.f;
        report.m_qualityFlag = INVALID;
      } catch (...) {
        const std::string message("Caught unknown exception while run QTest");

        if (!report.m_message.empty())
          report.m_message += " " + message;
        else
          report.m_message = message;

        report.m_quality = 0.f;
        report.m_qualityFlag = INVALID;
      }
    }

    //-------------------------------------------------------------------------------------------------

    void QualityTest::setLimits(float warning, float error) {
      
      if (warning < 0.f || error > 1.f || warning < error) {
        dqm_error("QualityTest::setLimits: Wrong limits provided (warning = {0}, error = {1})!", warning, error);
        throw StatusCodeException(STATUS_CODE_INVALID_PARAMETER);
      }

      m_warningLimit = warning;
      m_errorLimit = error;
    }

    //-------------------------------------------------------------------------------------------------

    float QualityTest::warningLimit() const {
      return m_warningLimit;
    }

    //-------------------------------------------------------------------------------------------------

    float QualityTest::errorLimit() const {
      return m_errorLimit;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    float QualityTest::defaultWarningLimit() {
      return m_defaultWarningLimit;
    }
    
    //-------------------------------------------------------------------------------------------------
    
    float QualityTest::defaultErrorLimit() {
      return m_defaultErrorLimit;
    }

    //-------------------------------------------------------------------------------------------------

    void QualityTest::fillBasicInfo(MonitorElement* monitorElement, QualityTestReport &report) const {
      report.m_qualityTestName = this->name();
      report.m_qualityTestType = this->type();
      report.m_qualityTestDescription = this->description();
      report.m_monitorElementType = monitorElement ? monitorElement->type() : "";
      report.m_monitorElementName = monitorElement ? monitorElement->name() : "";
      report.m_monitorElementPath = monitorElement ? monitorElement->path() : "";
      report.m_quality = 0.f;
      report.m_message = "";
      report.m_qualityFlag = UNDEFINED;
      report.m_extraInfos.clear();
    }
  }
}
