  /// \file DQMMonitorElement.h
/*
 *
 * DQMMonitorElement.h header template automatically generated by a class generator
 * Creation date : jeu. sept. 4 2014
 *
 * This file is part of DQM4HEP libraries.
 * 
 * DQM4HEP is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * based upon these libraries are permitted. Any copy of these libraries
 * must include this copyright notice.
 * 
 * DQM4HEP is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with DQM4HEP.  If not, see <http://www.gnu.org/licenses/>.
 * 
 * @author Ete Remi
 * @copyright CNRS , IPNL
 */


#ifndef DQMMONITORELEMENT_H
#define DQMMONITORELEMENT_H

// -- dqm4hep headers
#include "dqm4hep/DQM4HEP.h"
#include "dqm4hep/DQMQualityTest.h"
#include "dqm4hep/DQMPath.h"

// -- root headers
#include <TObject.h>
#include <TGraph.h>
#include <TPaveText.h>
#include <Rtypes.h>

// -- xdrstream headers
#include "xdrstream/xdrstream.h"

namespace dqm4hep
{

class DQMModuleApi;
class DQMStorage;
class DQMDirectory;
class DQMQualityTest;
class DQMQualityTestResult;
class DQMMonitorElementManager;

/** DQMMonitorElement class.
 *  Base class for plots and scalars that can be monitored via a user module.
 *  Monitor elements can be booked via the DQMModuleApi in order to be published.
 *
 *  @author R.Ete
 */ 
class DQMMonitorElement : public xdrstream::Streamable
{
	friend class DQMModuleApi;
	friend class DQMStorage;
	friend class DQMDirectory;
	friend class DQMMonitorElementManager;

public:
	/** Default constructor
	 */
	DQMMonitorElement(DQMMonitorElementType type = NO_ELEMENT_TYPE,
			const std::string &name = "", const std::string &title = "",
			const std::string &moduleName = "");

	/** DQMMonitorElement with type, name and title
	 */
	DQMMonitorElement(TObject *pObject, DQMMonitorElementType type,
		const std::string &name, const std::string &title, const std::string &moduleName);

	/** Destructor
	 */
	virtual ~DQMMonitorElement();

	/** Returns the element type
	 */
	DQMMonitorElementType getType() const;

	/** Returns the name of the element
	 */
	virtual const std::string &getName() const;

	/** Get the path (directory) of this element
	 */
	const DQMPath &getPath() const;

	/** Set the path (directory) for this element
	 */
	void setPath(const DQMPath &path);

	/** Get the collector name where this element is collected (or will be)
	 *  Set by the framework after collect
	 */
	const std::string &getCollectorName() const;

	/** Set the collector name where this element is collected (or will be)
	 *  Used by the framework after collect.
	 */
	void setCollectorName(const std::string &collectorName);

	/** Get the module name that has booked this element
	 */
	const std::string &getModuleName() const;

	/** Whether the monitor element is a histogram
	 */
	bool isHistogram() const;

	/** Whether the monitor element is a scalar value
	 */
	bool isScalar() const;

	/** Returns the element quality
	 */
	DQMQuality getQuality() const;

	/** Set the element quality
	 */
	void setQuality(DQMQuality quality);

	/** Returns the reset policy of the element. See enumerator definition
	 */
	DQMResetPolicy getResetPolicy() const;

	/** Sets the reset policy of the element. See enumerator definition
	 */
	void setResetPolicy(DQMResetPolicy policy);

	/** Returns the title of the object
	 */
	virtual const std::string &getTitle() const;

	/** Sets the element title
		*/
	virtual void setTitle(const std::string &title);

	/** Returns the element description
	 */
	const std::string &getDescription() const;

	/** Sets the element description
		*/
	void setDescription(const std::string &description);

	/** Return the draw option
	 */
	const std::string &getDrawOption() const;

	/** Set the draw option
	 */
	void setDrawOption(const std::string &drawOption);

	/** Returns the current run for this monitor element
	 */
	unsigned int getRunNumber() const;

	/** Sets the current run number
	 */
	void setRunNumber(unsigned int runNumber);

	/** Sets whether the element has to be published at the next end of cycle.
	 *  This flag is not reset by the framework. The user is
	 *  responsible for publishing or not the element.
	 */
	void setToPublish(bool publish);

	/** Whether the element has to be published at the next end of cycle.
	 */
	bool isToPublish() const;

	/** Return the associated TObject that is monitored
	 */
	TObject *getObject() const;

	/** Return the monitor object after a dynamic cast of the asked type
	 */
	template <typename T>
	T *get() const;

	/** Return the quality test results map.
	 *  Key : quality test name, value : quality test result
	 */
	const DQMQualityTestResultMap &getQualityTestResults() const;

	/** Reset the element
	 */
	void reset();

	xdrstream::Status stream(xdrstream::StreamingMode mode, xdrstream::IODevice *pDevice,
			xdrstream::xdr_version_t version = 0);

private:
	/** Run the given quality test
	 */
	StatusCode runQualityTest(const std::string &qualityTestName);

	/** Run all the quality tests
	 */
	StatusCode runQualityTests();

	/** Add a quality test
	 */
	StatusCode addQualityTest(DQMQualityTest *pQualityTest);

	/** Remove a quality test
	 */
	StatusCode removeQualityTest(DQMQualityTest *pQualityTest);

	/** Remove a quality test
	 */
	StatusCode removeQualityTest(const std::string &qualityTestName);

	//members
	TObject                        *m_pObject;             ///< The monitored TObject

	std::string                     m_name;                ///< The element name
	std::string                     m_title;               ///< The element title
	std::string                     m_description;        ///< The element description
	std::string                     m_drawOption;         ///< The draw option to apply while drawing
	std::string                     m_moduleName;         ///< The module name that have booked this element
	std::string                     m_collectorName;      ///< The collector name that have collected this element

	DQMPath                         m_path;
	DQMMonitorElementType           m_type;                  ///< The element type
	DQMQuality                      m_quality;               ///< The element quality has defined by the user
	DQMResetPolicy                  m_resetPolicy;          ///< The reset policy. See enumerator definition

	unsigned int                    m_runNumber;           ///< The run number when the element is published
	bool                            m_toPublish;           ///< Whether the element has to be published at end of cycle

	std::map<std::string, DQMQualityTest*>       m_qualityTestMap;         /// The quality test map to perform
	std::map<std::string, DQMQualityTestResult>  m_qualityTestResultMap;  /// The quality test result list filled after performing the quality tests
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

/** TScalarObject class.
 *  Extension of a TObject for scalar values like int,
 *  float, double or string.
 */
template<typename T>
class TScalarObject : public TObject
{
public:

	/** Default constructor
	 */
	TScalarObject();

	/** Constructor with scalar value
	 */
	TScalarObject(const T &scalar);

	/** Destructor
	 */
	~TScalarObject();

	/** Clear the scalar object
	 */
	void Clear(Option_t *option = "");

	/** Draw the scalar object (using a TPaveText)
	 */
	void Draw(Option_t *option = "");

	/** The scalar value
	 */
	void Set(const T &value);

	/** Get the scalar value
	 */
	const T &Get() const;

	/** Convert the scalar value to std::string
	 */
	std::string ToString() const;

private:
	/** Initialize the scalar object (called only in C'tor)
	 */
	void Init();

	T            m_scalar;       ///< The scalar value
	TPaveText   *m_pPaveText;    ///< The pave text on which to draw the scalar value

	ClassDef(TScalarObject, 1);
};

typedef TScalarObject<dqm_int>     TScalarInt;
typedef TScalarObject<dqm_float>   TScalarReal;
typedef TScalarObject<dqm_float>   TScalarFloat;
typedef TScalarObject<dqm_double>  TScalarDouble;
typedef TScalarObject<dqm_short>   TScalarShort;
typedef TScalarObject<long>       TScalarLong;
typedef TScalarObject<Long64_t>    TScalarLong64_t;
typedef TScalarObject<std::string> TScalarString;

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

/** TDynamicGraph class
 *
 *  A TGraph with a dynamic x range axis
 */
class TDynamicGraph : public TGraph
{
public:
	enum
	{
		kDynamicRange  = BIT(14),
		kShrinkToRange = BIT(15)
	};

	/** Constructor
	 */
	TDynamicGraph();

	/** Destructor
	 */
	~TDynamicGraph();

	/** Set the range length.
	 *  If kDynamicRange and kShrinkToRange bit are set
	 *  then all points that do not fit the x range are removed
	 */
	void SetRangeLength(Double_t rangeLength);

	/** Add a point at end of array.
	 *  Remove points that do not fit the dynamic range
	 *  if kDynamicRange and kShrinkToRange bit are set
	 */
	void AddPoint(Double_t x, Double_t y);

	/** Draw dynamic graph. Apply dynamic range if bit set
	 */
	void Draw(Option_t *option = "");

private:
	/**
	 */
	void ShrinkToRange();

private:
	Double_t       m_rangeLength;

	ClassDef(TDynamicGraph, 1);
};

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

template <typename T>
T *DQMMonitorElement::get() const
{
	return dynamic_cast<T*>(m_pObject);
}

//-------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------------------------------

template <typename T>
TScalarObject<T>::TScalarObject()
{
	Init();
}

//-------------------------------------------------------------------------------------------------

template <typename T>
TScalarObject<T>::TScalarObject(const T &scalar) :
 m_scalar(scalar),
 m_pPaveText(0)
{

}

//-------------------------------------------------------------------------------------------------

template <typename T>
TScalarObject<T>::~TScalarObject()
{
	if(0 != m_pPaveText)
	{
		delete m_pPaveText;
		m_pPaveText = 0;
	}
}

//-------------------------------------------------------------------------------------------------

template <typename T>
void TScalarObject<T>::Clear(Option_t *option)
{
	if(0 != m_pPaveText)
		m_pPaveText->Clear(option);

	m_scalar = 0;
}

//-------------------------------------------------------------------------------------------------

template <typename T>
void TScalarObject<T>::Draw(Option_t *option)
{
	std::string scalarStr = ToString();

	if(0 == m_pPaveText)
		m_pPaveText = new TPaveText(0.1, 0.1, 0.9, 0.9, "NDC");

	m_pPaveText->Clear();
	m_pPaveText->AddText(scalarStr.c_str());
	m_pPaveText->Draw(option);
}

//-------------------------------------------------------------------------------------------------

template <typename T>
void TScalarObject<T>::Set(const T &value)
{
	m_scalar = value;
}

//-------------------------------------------------------------------------------------------------

template <typename T>
const T &TScalarObject<T>::Get() const
{
	return m_scalar;
}

//-------------------------------------------------------------------------------------------------

template <typename T>
std::string TScalarObject<T>::ToString() const
{
	std::stringstream ss;
	ss << m_scalar;
	return ss.str();
}

//-------------------------------------------------------------------------------------------------

template <typename T>
void TScalarObject<T>::Init()
{
	m_scalar = 0;
	m_pPaveText = 0;
}

//-------------------------------------------------------------------------------------------------
// template specialization for std::string
//-------------------------------------------------------------------------------------------------

template <>
inline void TScalarObject<std::string>::Clear(Option_t *option)
{
	if(NULL != m_pPaveText)
		m_pPaveText->Clear(option);

	m_scalar.clear();
}

//-------------------------------------------------------------------------------------------------

template <>
inline std::string TScalarObject<std::string>::ToString() const
{
	return m_scalar;
}

//-------------------------------------------------------------------------------------------------

template <>
inline void TScalarObject<std::string>::Init()
{
	m_scalar = "";
	m_pPaveText = 0;
}

} 

#endif  //  DQMMONITORELEMENT_H
