#include "http_request.h"


                
                /************************************************************/
                /********************REQUEST PARSER TOOLS********************/
                /************************************************************/

#include <boost/asio.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

using boost::spirit::qi::parse;
using namespace boost::spirit;
using namespace boost::spirit::qi;
using namespace boost::spirit::ascii;



//parsing symbol table
struct method_: symbols<char, short>
{
    method_()
    {
        add
            ("GET", GET_M)
            ("POST", POST_M)
            ("HEAD", HEAD_M)
            ("PUT", PUT_M)
            ("OPTIONS", OPTIONS_M)
            ("DELETE", DELETE_M)
            ("TRACE", TRACE_M)
            ("CONNECT", CONNECT_M);

    }
} method;


                /************************************************************/


cRequest::cRequest(std::istream &stream)
:m_Method(INDETERMINATE_M)
{
    base_iterator_type in_begin(stream);
    // convert input iterator to forward iterator, usable by spirit parser
    m_Fwd_begin = make_default_multi_pass(in_begin);
};

void cRequest::ParseRequest()
{
    short method_;
    bool result = parse(m_Fwd_begin, m_Fwd_end,
        /////grammar
       (
        method
        >>' '
        >>(+~boost::spirit::ascii::char_(' '))
        >>' '
        >>(+~boost::spirit::ascii::char_('\r'))
        >>lit("\r\n")
        >>(*(boost::spirit::ascii::char_)))
        //////
        ,
        method_,
        m_Resource,
        m_Version,
        m_Headers
        );
        m_Method = (REQ_METHOD)method_;
    if(false == result)
        throw ;

};

void cRequest::ParseBody()
{


};