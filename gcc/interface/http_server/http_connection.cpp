
#include "http_connection.h"
#include "http_request.h"
#include "http_response.h"
#include "connection_manager.h"
#include "pagebuilder.h"
#include "../../engine/session.h"
#include "../../engine/logger.h"
#include "../../engine/command.h"

#include <iostream>
#include <boost/bind.hpp>


namespace http_server
{

typedef std::pair<unsigned int, cSession*> ses_pair;

void cHttpConnection::HandleClient()
{
    //read
    boost::asio::async_read_until(m_Socket, m_RequestBuf, "\r\n",
        boost::bind(&cHttpConnection::HandleRequest, shared_from_this(),
			boost::asio::placeholders::error));
};


void cHttpConnection::HandleRequest(const boost::system::error_code& error)
{
    if(!error)
    {
        std::istream is(&m_RequestBuf);
        cRequest _request(is);
        if(false == _request.ParseRequest())
		{
			std::string request_str;
			is>>request_str;
			throw std::runtime_error(CONTEXT_STR + "failed to parse request :" + request_str);
		}
		m_HttpVersion = (_request.GetVersion() == HTTP_VER1)? 1 : 0;
        switch(_request.GetMethod())
        {
        case GET_M:
            if(_request.GetResource() == "/")
            {
				//add new session
				unsigned int ses_id = cHttpConnection::GetRandUniqueId();
				cHttpConnection::s_Sessions.insert(ses_pair(ses_id, new cSession(ses_id)));

				//send response
                cResponse response(m_ResponseBuf);
				const std::string index_page = cPageBuilder::GetInstance()->GetIndexPage(ses_id);
                response.BuildResponse(OK, index_page);

                boost::asio::async_write(m_Socket, m_ResponseBuf,
                    boost::bind(&cHttpConnection::HandleWriteResponse, shared_from_this(),
                    boost::asio::placeholders::error));
            }
			else
			{
                cResponse response(m_ResponseBuf);
				unsigned int ses_id = _request.GetSessionId();

				if(cHttpConnection::s_Sessions.find(ses_id) != cHttpConnection::s_Sessions.end())
				{

					cSession* session = cHttpConnection::s_Sessions[ses_id];
					if(session->GetState() == STATE_FREE)
					{
						//TODO -- shared_ptr ??? create command
						cCommand *command = new cCommand(_request.GetCommand(), session->GetResult());
						int runtime_estimation = command.EstimateRunTime(s_Estimator);

						if( runtime_estimation <= 360/*seconds*/)
						{
							session->RunCommand(command);
							response.BuildResponse(OK, cPageBuilder::GetInstance()->GetPage(session->GetResult(), ses_id));
						}
						else
						{
							session->ScheduleCommand(command);
							response.BuildResponse(OK, cPageBuilder::GetInstance()->GetLoadingPage(runtime_estimation, ses_id));
						}
					}
					else if(session->GetState() == STATE_RESULT_PENDING)
					{
						response.BuildResponse(OK, cPageBuilder::GetInstance()->GetPage(session->GetResult(), ses_id));
					}

					boost::asio::async_write(m_Socket, m_ResponseBuf,
						boost::bind(&cHttpConnection::HandleWriteResponse, shared_from_this(),
						boost::asio::placeholders::error));
				}
				else
				{
					//add new session
					cHttpConnection::s_Sessions.insert(ses_pair(ses_id, new cSession(ses_id)));

					const std::string index_page  = cPageBuilder::GetInstance()->GetIndexPage(
						ses_id);
                	response.BuildResponse(OK, index_page);
				}
				boost::asio::async_write(m_Socket, m_ResponseBuf,
               		boost::bind(&cHttpConnection::HandleWriteResponse, shared_from_this(),
                   	boost::asio::placeholders::error));
			}
            break;
        case POST_M:
            break;
        case HEAD_M:
            break;
        case PUT_M:
            break;
        case OPTIONS_M:
            break;
        case DELETE_M:
            break;
        case TRACE_M:
            break;
        case CONNECT_M:
            break;
        default:
            break;

        }
    }
    else
    {
		throw std::runtime_error(CONTEXT_STR + error.message());
    }
};

void cHttpConnection::HandleWriteResponse(const boost::system::error_code& error)
{
    if(error)
    {
		throw std::runtime_error(CONTEXT_STR + error.message());
    }
	else
	{
		if(m_HttpVersion == true)	//persistent connection
		{
			HandleClient();
		}
		else
		{
			m_ConnectionManager.StopConnection(shared_from_this());
		}

	}
};

void cHttpConnection::Stop()
{
	boost::system::error_code error;
	m_Socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both, error);
	m_Socket.close();
};

}

