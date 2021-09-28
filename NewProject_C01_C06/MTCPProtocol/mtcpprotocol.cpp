
#include "mtcp_module.h"

using namespace MTcp;

PYBIND11_MODULE(MTCPProtocol, m)
{
    m.doc() = "MTCPProtocol module";

    py::class_<GENL, std::unique_ptr<GENL, py::nodelete>> mtcp(m, "MTCPProtocol");
    mtcp.def(py::init(
                 [](const std::string& path) { return std::unique_ptr<GENL, py::nodelete>(GENL::getInstance(path)); }))
        .def("open", &GENL::CFrameBase::open)
        .def("isOpen", &GENL::CFrameBase::isOpen)
        .def("SendGENL", &GENL::SendGENL)
        .def("rspType", &GENL::rspType)
        .def("setTestCSVPath", &GENL::setTestCSVPath)
        .def("setLogPath", &GENL::setLogPath)
        .def("version", &GENL::version);

    py::class_<MTCP_ProtocolException, std::unique_ptr<MTCP_ProtocolException, py::nodelete>> mtcpErr(
        m, "MTCP_ProtocolException");
    mtcpErr
        .def(py::init([]() {
            return std::unique_ptr<MTCP_ProtocolException, py::nodelete>(MTCP_ProtocolException::GetInstance(NULL, ""));
        }))
        .def("getErrCode", &MTCP_ProtocolException::getErrCode)
        .def("getErrStr", &MTCP_ProtocolException::getErrStr);

    py::enum_<GENL::DirectionType>(mtcp, "DirectionType")
        .value("REQUEST", GENL::DirectionType::REQUEST)
        .value("REPORT", GENL::DirectionType::REPORT)
        .export_values();
}
