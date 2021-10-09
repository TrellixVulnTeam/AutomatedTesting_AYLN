
#include "ConfigParse.h"
#include "Util.h"
#include "LogHelper.h"
#include <QApplication>
#include <QDebug>

ConfigParse& ConfigParse::getInstance()
{
    static ConfigParse instance;
    return instance;
}

void ConfigParse::procError(const std::string& nodeStr)
{
    std::ostringstream errStr;
    errStr << nodeStr << " is invalid in UI configure file: " << m_pathOfUIConf << std::endl;
    LOG_FATAL("[Main] %s", errStr.str().c_str());
    throw std::runtime_error(errStr.str());
}

void ConfigParse::prestrain(const std::string& pathOfUIConf)
{
    m_pathOfUIConf = pathOfUIConf;
    Util::Replace(m_pathOfUIConf, "\\\\", "/", 1);
    XmlNode rootNode;
    bool flag = XmlNode::ParseXml(m_pathOfUIConf, rootNode);
    if (!flag) {
        std::ostringstream errStr;
        errStr << __func__ << " parse UI configure file: " << m_pathOfUIConf << " failed" << std::endl;
        LOG_FATAL("[Main] %s", errStr.str().c_str());
        throw std::runtime_error(errStr.str());
    }

    XmlNode configurationNode = rootNode.Child(KConfiguration);
    if (configurationNode.isValid()) {
        XmlNode _node = configurationNode.Child(KProjectName);
        m_projectName = _node.Value<std::string>();
        _node = configurationNode.Child(KVersion);
        m_version = _node.Value<std::string>();
        _node = configurationNode.Child(KConfig);
        m_config = _node.Value<std::string>();

        _node = configurationNode.Child(KLogPath);
        m_logPath = _node.Value<std::string>();
        Util::MakeNDir(m_logPath);

#ifdef Q_OS_MAC
        QString appPath = QApplication::applicationDirPath().replace("MacOS", "Resources/");
#else
        QString appPath = QApplication::applicationDirPath();
#endif
        _node = configurationNode.Child(KFlowCsvPath);
        m_flowCsvPath = _node.Value<std::string>();
        Util::Replace(m_flowCsvPath, "[App]", appPath.toStdString(), 0);

        _node = configurationNode.Child(KSpecCsvPath);
        m_specCsvPath = _node.Value<std::string>();
        Util::Replace(m_specCsvPath, "[App]", appPath.toStdString(), 0);

        _node = configurationNode.Child(KPositionCsvPath);
        m_positionCsvPath = _node.Value<std::string>();
        Util::Replace(m_positionCsvPath, "[App]", appPath.toStdString(), 0);

        _node = configurationNode.Child(KLotName);
        m_lotName = _node.Value<std::string>();

        _node = configurationNode.Child(KMtcpIP);
        if (_node.isValid()) {
            m_mtcpIP = _node.Value<std::string>();
        }

        _node = configurationNode.Child(KMtcpPort);
        if (_node.isValid()) {
            m_mtcpPort = _node.Value<std::string>();
        }

        _node = configurationNode.Child(KRMS);
        m_rms = _node.Value<double>();

        LOG_INFO("");
        LOG_INFO("[Main] ***********************");
        LOG_INFO("[Main] Config xml path: %s", m_pathOfUIConf.c_str());
        LOG_INFO("[Main] ProjectName: %s", m_projectName.c_str());
        LOG_INFO("[Main] Version: %s", m_version.c_str());
        LOG_INFO("[Main] Config: %s", m_config.c_str());
        LOG_INFO("[Main] LogPath: %s", m_logPath.c_str());
        LOG_INFO("[Main] FlowCsvPath: %s", m_flowCsvPath.c_str());
        LOG_INFO("[Main] SpecCsvPath: %s", m_specCsvPath.c_str());
        LOG_INFO("[Main] Position: %s", m_positionCsvPath.c_str());
        LOG_INFO("[Main] LotName: %s", m_lotName.c_str());
        LOG_INFO("[Main] RMS: %f", m_rms);

        XmlNode device = configurationNode.Child(KDevice);
        if (device.isValid()) {
            const std::vector<XmlNode> unitDevices = device.GetChilds(KUnit);
            foreach (XmlNode udic, unitDevices) {
                std::string target = udic.Attribute<std::string>("target");
                std::string path = udic.Attribute<std::string>("path");
                int num = udic.Attribute<int>("num");
                std::string ref = udic.Attribute<std::string>("ref");
                int timeout = udic.Attribute<int>("timeout");
                std::string suffix = udic.Attribute<std::string>("suffix");
                bool exist = udic.Attribute<bool>("exist");
                bool pro = udic.Attribute<bool>("Property");

                DeviceInfo dinfo(target, path, num, ref, timeout, suffix, exist, pro);
                m_unitDevicesMap.insert(std::pair<std::string, DeviceInfo>(ref, dinfo));
            }

            const std::vector<XmlNode> groupDevices = device.GetChilds(KGroup);
            foreach (XmlNode udic, groupDevices) {
                std::string target = udic.Attribute<std::string>("target");
                std::string path = udic.Attribute<std::string>("path");
                int num = udic.Attribute<int>("num");
                std::string ref = udic.Attribute<std::string>("ref");
                int timeout = udic.Attribute<int>("timeout");
                std::string suffix = udic.Attribute<std::string>("suffix");
                bool exist = udic.Attribute<bool>("exist");
                bool pro = udic.Attribute<bool>("Property");

                m_groupDevicesMap.insert(std::pair<std::string, DeviceInfo>(
                    ref, DeviceInfo(target, path, num, ref, timeout, suffix, exist, pro)));
            }
        }

        m_unitCount = configurationNode.Child(KUnitCount).Value<int>();
        m_machineID = configurationNode.Child(KMachineID).Value<std::string>();
        m_slot = configurationNode.Child(KSlot).Value<std::string>();
        m_alpha = configurationNode.Child(KAlpha).Value<int>();
        m_beta = configurationNode.Child(KBeta).Value<int>();
        m_gamma = configurationNode.Child(KGamma).Value<int>();
        m_xp = configurationNode.Child(KXp).Value<int>();
        m_xs = configurationNode.Child(KXs).Value<int>();
        m_y = configurationNode.Child(KY).Value<int>();
        m_z = configurationNode.Child(KZ).Value<int>();
        m_timeStep = configurationNode.Child(KTimeStep).Value<double>();
        m_Mtcp = configurationNode.Child(KMtcp).Value<bool>();
        m_Mes = configurationNode.Child(KMes).Value<bool>();

        LOG_INFO("[Main] UnitCount: %d", m_unitCount);
        LOG_INFO("[Main] MachineID: %s", m_machineID.c_str());
        LOG_INFO("[Main] Solt: %s", m_slot.c_str());
        LOG_INFO("[Main] Alpha: %d", m_alpha);
        LOG_INFO("[Main] Beta: %d", m_beta);
        LOG_INFO("[Main] Gamma: %d", m_gamma);
        LOG_INFO("[Main] Xp: %d", m_xp);
        LOG_INFO("[Main] Xs: %d", m_xs);
        LOG_INFO("[Main] Y: %d", m_y);
        LOG_INFO("[Main] Z: %d", m_z);
        LOG_INFO("[Main] TimeStep: %.7f", m_timeStep);
        LOG_INFO("[Main] MTCP: %s", m_Mtcp ? "True" : "False");
        LOG_INFO("[Main] MtcpIP: %s", m_mtcpIP.c_str());
        LOG_INFO("[Main] MtcpPort: %s", m_mtcpPort.c_str());
        LOG_INFO("[Main] MES: %s", m_Mes ? "True" : "False");
        LOG_INFO("[Main] ***********************\n");

        std::vector<XmlNode> targetNode = configurationNode.Child(KIsChoose).GetChilds("Value");
        int vNum = configurationNode.Child(KIsChoose).Attribute<int>("count");
        vNum = vNum < targetNode.size() ? vNum : targetNode.size();

        for (int i = 0; i < vNum; i++) {
            bool v = targetNode[i].Value<bool>();
            m_chooseVec.push_back(v);
        }

        targetNode = configurationNode.Child(KGroupOrder).GetChilds("Value");
        vNum = configurationNode.Child(KGroupOrder).Attribute<int>("count");
        vNum = vNum < targetNode.size() ? vNum : targetNode.size();

        for (int i = 0; i < vNum; i++) {
            std::string v = targetNode[i].Value<std::string>();
            m_groupOrderVec.push_back(v);
        }

        XmlNode angleVelNode = configurationNode.Child(KAngleAndVel);
        if (angleVelNode.isValid()) {
            std::vector<XmlNode> itemNodes = angleVelNode.GetChilds("Item");
            foreach (XmlNode avItem, itemNodes) {
                std::vector<float> valueVec;
                std::string name = avItem.Attribute<std::string>("name");
                valueVec.push_back(avItem.Attribute<float>("angle"));
                valueVec.push_back(avItem.Attribute<float>("velcoty"));
                m_angleAndVelMap.insert(std::pair<std::string, std::vector<float>>(name, valueVec));
            }
        }
    }
}

ConfigParse::ConfigParse()
{
    //    LOG_INFO("[Main] Initialize ConfigParse");
    qDebug() << "[Main] Initialize ConfigParse";
}

void ConfigParse::setTestInfo(const std::string& node, bool value)
{
    std::unique_lock<std::mutex> lock(_mutex);
    XmlNode rootNode;
    XmlNode::ParseXml(m_pathOfUIConf, rootNode);
    XmlNode configurationNode = rootNode.Child(KConfiguration);
    if (configurationNode.isValid()) {
        XmlNode testInfo = configurationNode.Child(node);
        if (testInfo.isValid()) {
            testInfo.SetValue(value);
        } else {
            configurationNode.AddChild(node, value);
        }

        if (node == KMtcp) {
            m_Mtcp = value;
            LOG_INFO("[Main] Change MTCP to: %d", m_Mtcp ? "True" : "False");
        } else if (node == KMes) {
            m_Mes = value;
            LOG_INFO("[Main] Change MES to: %d", m_Mes ? "True" : "False");
        }
    } else {
        procError(KConfiguration);
        return;
    }
    rootNode.Save(m_pathOfUIConf);
}

void ConfigParse::setTestInfo(const std::string& node, const int value)
{
    std::unique_lock<std::mutex> lock(_mutex);
    XmlNode rootNode;
    XmlNode::ParseXml(m_pathOfUIConf, rootNode);
    XmlNode configurationNode = rootNode.Child(KConfiguration);
    if (configurationNode.isValid()) {
        XmlNode testInfo = configurationNode.Child(node);
        if (testInfo.isValid()) {
            testInfo.SetValue(value);
        } else {
            configurationNode.AddChild(node, value);
        }

        if (node == KUnitCount) {
            m_unitCount = value;
            LOG_INFO("[Main] Change UnitCount to: %d", m_unitCount);
        } else if (node == KAlpha) {
            m_alpha = value;
            LOG_INFO("[Main] Change Alpha to: %d", m_unitCount);
        } else if (node == KBeta) {
            m_beta = value;
            LOG_INFO("[Main] Change Beta to: %d", m_unitCount);
        } else if (node == KGamma) {
            m_gamma = value;
            LOG_INFO("[Main] Change Gamma to: %d", m_unitCount);
        } else if (node == KXp) {
            m_xp = value;
            LOG_INFO("[Main] Change Xp to: %d", m_unitCount);
        } else if (node == KXs) {
            m_xs = value;
            LOG_INFO("[Main] Change Xs to: %d", m_unitCount);
        } else if (node == KY) {
            m_y = value;
            LOG_INFO("[Main] Change Y to: %d", m_unitCount);
        } else if (node == KZ) {
            m_z = value;
            LOG_INFO("[Main] Change Z to: %d", m_unitCount);
        }
    } else {
        procError(KConfiguration);
        return;
    }
    rootNode.Save(m_pathOfUIConf);
}

void ConfigParse::setTestInfo(const std::string& node, const double value)
{
    std::unique_lock<std::mutex> lock(_mutex);
    XmlNode rootNode;
    XmlNode::ParseXml(m_pathOfUIConf, rootNode);
    XmlNode configurationNode = rootNode.Child(KConfiguration);
    if (configurationNode.isValid()) {
        XmlNode testInfo = configurationNode.Child(node);
        if (testInfo.isValid()) {
            testInfo.SetValue(value);
        } else {
            configurationNode.AddChild(node, value);
        }

        if (node == KTimeStep) {
            m_timeStep = value;
            LOG_INFO("[Main] Change TimeStep to: %.7f", m_unitCount);
        } else if (node == KRMS) {
            m_rms = value;
            LOG_INFO("[Main] Change RMS to: %f", m_rms);
        }
    } else {
        procError(KConfiguration);
        return;
    }
    rootNode.Save(m_pathOfUIConf);
}

void ConfigParse::setTestInfo(const std::string& node, const std::string& value)
{
    std::unique_lock<std::mutex> lock(_mutex);
    XmlNode rootNode;
    XmlNode::ParseXml(m_pathOfUIConf, rootNode);
    XmlNode configurationNode = rootNode.Child(KConfiguration);
    if (configurationNode.isValid()) {
        XmlNode testInfo = configurationNode.Child(node);
        if (testInfo.isValid()) {
            testInfo.SetValue(value);
        } else {
            configurationNode.AddChild(node, value);
        }

        if (node == KProjectName) {
            m_projectName = value;
            LOG_INFO("[Main] Change ProjectName to: %s", value.c_str());
        } else if (node == KMachineID) {
            m_machineID = value;
            LOG_INFO("[Main] Change MachineID to: %s", value.c_str());
        } else if (node == KSlot) {
            m_slot = value;
            LOG_INFO("[Main] Change Slot to: %s", value.c_str());
        } else if (node == KUnitCount) {
            m_unitCount = std::stoi(value);
            LOG_INFO("[Main] Change UnitCount to: %s", value.c_str());
        } else if (node == KAlpha) {
            m_alpha = std::stoi(value);
            LOG_INFO("[Main] Change Alpha to: %s", value.c_str());
        } else if (node == KBeta) {
            m_beta = std::stoi(value);
            LOG_INFO("[Main] Change Beta to: %s", value.c_str());
        } else if (node == KGamma) {
            m_gamma = std::stoi(value);
            LOG_INFO("[Main] Change Gamma to: %s", value.c_str());
        } else if (node == KXp) {
            m_xp = std::stoi(value);
            LOG_INFO("[Main] Change Xp to: %s", value.c_str());
        } else if (node == KXs) {
            m_xs = std::stoi(value);
            LOG_INFO("[Main] Change Xs to: %s", value.c_str());
        } else if (node == KY) {
            m_y = std::stoi(value);
            LOG_INFO("[Main] Change Y to: %s", value.c_str());
        } else if (node == KZ) {
            m_z = std::stoi(value);
            LOG_INFO("[Main] Change Z to: %s", value.c_str());
        } else if (node == KTimeStep) {
            m_timeStep = std::stod(value);
            LOG_INFO("[Main] Change TimeStep to: %s", value.c_str());
        } else if (node == KLogPath) {
            m_logPath = value;
            LOG_INFO("[Main] Change LogPath to: %s", value.c_str());
        } else if (node == KFlowCsvPath) {
            m_flowCsvPath = value;
            LOG_INFO("[Main] Change FlowCsvPath to: %s", value.c_str());
        } else if (node == KSpecCsvPath) {
            m_specCsvPath = value;
            LOG_INFO("[Main] Change SpecCsvPath to: %s", value.c_str());
        } else if (node == KPositionCsvPath) {
            m_positionCsvPath = value;
            LOG_INFO("[Main] Change PositionCsvPath to: %s", value.c_str());
        } else if (node == KMtcpIP) {
            m_mtcpIP = value;
            LOG_INFO("[Main] Change MtcpIP to: %s", value.c_str());
        } else if (node == KMtcpPort) {
            m_mtcpPort = value;
            LOG_INFO("[Main] Change MtcpPort to: %s", value.c_str());
        } else if (node == KLotName) {
            m_lotName = value;
            LOG_INFO("[Main] Change LotName to: %s", value.c_str());
        }
    } else {
        procError(KConfiguration);
        return;
    }
    rootNode.Save(m_pathOfUIConf);
}

void ConfigParse::setTestInfo(const std::string& node, const std::vector<bool>& values)
{
    std::unique_lock<std::mutex> lock(_mutex);
    setTestInfoTemplate<bool>(node, values);
    if (node == KIsChoose) {
        m_chooseVec = static_cast<std::vector<bool>>(values);
    }
}

void ConfigParse::setTestInfo(const std::string& node, const std::vector<std::string>& values)
{
    std::unique_lock<std::mutex> lock(_mutex);
    setTestInfoTemplate<std::string>(node, values);
    if (node == KGroupOrder) {
        m_groupOrderVec = static_cast<std::vector<std::string>>(values);
    }
}
