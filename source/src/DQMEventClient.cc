  /// \file DQMEventClient.cc
/*
 *
 * DQMEventClient.cc source template automatically generated by a class generator
 * Creation date : mar. sept. 8 2015
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
 * @author Remi Ete
 * @copyright CNRS , IPNL
 */

// -- dqm4hep headers
#include "dqm4hep/DQMEventClient.h"
#include "dqm4hep/DQMDimEventClient.h"

namespace dqm4hep
{

DQMEventClient::DQMEventClient()
{
	// default is dim implementation
	m_pClientImp = new DQMDimEventClient();
}

//-------------------------------------------------------------------------------------------------

DQMEventClient::DQMEventClient(DQMEventClientImp *pClientImp) :
		m_pClientImp(pClientImp)
{
	/* nop */
}

//-------------------------------------------------------------------------------------------------

DQMEventClient::~DQMEventClient()
{
	delete m_pClientImp;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::setEventClientImp(DQMEventClientImp *pClientImp)
{
	if(NULL == pClientImp)
		return STATUS_CODE_INVALID_PTR;

	if(NULL != m_pClientImp)
		delete m_pClientImp;

	m_pClientImp = pClientImp;

	return STATUS_CODE_SUCCESS;
}

//-------------------------------------------------------------------------------------------------

DQMEventClientImp *DQMEventClient::getEventClientImp() const
{
	return m_pClientImp;
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::setCollectorName(const std::string &collectorName)
{
	return m_pClientImp->setCollectorName(collectorName);
}

//-------------------------------------------------------------------------------------------------

const std::string &DQMEventClient::getCollectorName() const
{
	return m_pClientImp->getCollectorName();
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::connectToService()
{
	return m_pClientImp->connectToService();
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::disconnectFromService()
{
	return m_pClientImp->disconnectFromService();
}

//-------------------------------------------------------------------------------------------------

bool DQMEventClient::isConnectedToService() const
{
	return m_pClientImp->isConnectedToService();
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::sendEvent(const DQMEvent *const pEvent)
{
	return m_pClientImp->sendEvent(pEvent);
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::setEventStreamer(DQMEventStreamer *pEventStreamer)
{
	return m_pClientImp->setEventStreamer(pEventStreamer);
}

//-------------------------------------------------------------------------------------------------

const DQMEventStreamer *DQMEventClient::getEventStreamer() const
{
	return m_pClientImp->getEventStreamer();
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::setMaximumQueueSize(unsigned int queueSize)
{
	return m_pClientImp->setMaximumQueueSize(queueSize);
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::clearQueue()
{
	return m_pClientImp->clearQueue();
}

//-------------------------------------------------------------------------------------------------

void DQMEventClient::setSubEventIdentifier(const std::string &identifier)
{
	m_pClientImp->setSubEventIdentifier(identifier);
}

//-------------------------------------------------------------------------------------------------

const std::string &DQMEventClient::getSubEventIdentifier() const
{
	return 	m_pClientImp->getSubEventIdentifier();
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::queryEvent(DQMEvent *&pEvent, int timeout)
{
	return m_pClientImp->queryEvent(pEvent, timeout);
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::queryEvent()
{
	return m_pClientImp->queryEvent();
}

//-------------------------------------------------------------------------------------------------

StatusCode DQMEventClient::takeEvent(DQMEvent *&pEvent)
{
	return m_pClientImp->takeEvent(pEvent);
}

//-------------------------------------------------------------------------------------------------

void DQMEventClient::setUpdateMode(bool updateMode)
{
	return m_pClientImp->setUpdateMode(updateMode);
}

//-------------------------------------------------------------------------------------------------

bool DQMEventClient::updateMode() const
{
	return m_pClientImp->updateMode();
}

}

