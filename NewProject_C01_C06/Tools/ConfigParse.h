#pragma once

#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <QMap>
#include <QRegExp>
#include <mutex>
#include "User.h"

#include "XmlHelper.h"

#define CFG_PARSE ConfigParse::getInstance()

static std::string KConfiguration = "Configuration";
static std::string KDevice = "Device";
static std::string KUnit = "Unit";
static std::string KGroup = "Group";
static std::string KProjectName = "ProjectName";
static std::string KVersion = "Version";
static std::string KConfig = "Config";
static std::string KLogPath = "LogPath";
static std::string KUnitCount = "UnitCount";
static std::string KOffset = "Offset";
static std::string KMachineID = "MachineID";
static std::string KSlot = "Slot";
static std::string KIsChoose = "IsChoose";
static std::string KGroupOrder = "GroupOrder";

static std::string KAlpha = "Alpha";
static std::string KBeta = "Beta";
static std::string KGamma = "Gamma";
static std::string KXp = "Xp";
static std::string KXs = "Xs";
static std::string KZ = "Z";
static std::string KY = "Y";
static std::string KTimeStep = "TimeStep";

static std::string KAngleAndVel = "AngleAndVel";

static std::string KFlowCsvPath = "FlowCsvPath";
static std::string KSpecCsvPath = "SpecCsvPath";
static std::string KPositionCsvPath = "PositionCsvPath";

static std::string KMtcp = "MTCP";
static std::string KMtcpIP = "MtcpIP";
static std::string KMtcpPort = "MtcpPort";
static std::string KLotName = "LotName";
static std::string KRMS = "RMS";
static std::string KMes = "MES";

static std::string KPID = "WatcherPID";

struct DeviceInfo
{
    DeviceInfo()
    {
        error = "NULL struct";
    }
    DeviceInfo(std::string _target, std::string _path, int _num, std::string _ref, int _timeout, std::string _suffix,
               bool _exist, bool _property)
        : target(_target), path(_path), num(_num), ref(_ref), timeout(_timeout), suffix(_suffix), exist(_exist),
          Property(_property)
    {
    }

    std::string target;
    std::string path;
    int num;
    std::string ref;
    int timeout;
    std::string suffix;
    bool exist;
    bool Property;
    std::string error = "";
};

class ConfigParse
{
public:
    static ConfigParse& getInstance();
    void prestrain(const std::string& pathOfUIConf);

    void setTestInfo(const std::string& node, const std::string& value);
    void setTestInfo(const std::string& node, bool value);
    void setTestInfo(const std::string& node, const int value);
    void setTestInfo(const std::string& node, const double value);
    void setTestInfo(const std::string& node, const std::vector<bool>& values);
    void setTestInfo(const std::string& node, const std::vector<std::string>& values);
    void setUnitInfo(const std::vector<DeviceInfo>& info);

    int getUnitCount() const
    {
        return m_unitCount;
    }

    int getOffset() const
    {
        return m_offset;
    }

    std::string getSlot() const
    {
        return m_slot;
    }

    std::string getMachineID() const
    {
        return m_machineID;
    }

    std::string getProjectName() const
    {
        return m_projectName;
    }

    std::string getPathOfUIConf() const
    {
        return m_pathOfUIConf;
    }

    std::vector<bool> getChooseVec() const
    {
        return m_chooseVec;
    }

    bool getChooseItem(int index) const
    {
        return m_chooseVec[index];
    }

    std::vector<std::string> getGroupOrderVec() const
    {
        return m_groupOrderVec;
    }

    std::string getGroupOrderItem(int index) const
    {
        return m_groupOrderVec[index];
    }

    std::map<std::string, DeviceInfo> getUnitDevicesMap() const
    {
        return m_unitDevicesMap;
    }

    DeviceInfo getUnitDevicesKey(std::string& key)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (m_unitDevicesMap.find(key) != m_unitDevicesMap.end())
            return m_unitDevicesMap[key];
        return DeviceInfo();
    }

    std::vector<DeviceInfo> getUnitDevicesTarget(std::string& target)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        std::vector<DeviceInfo> vec;
        for (std::map<std::string, DeviceInfo>::iterator it = m_unitDevicesMap.begin(); it != m_unitDevicesMap.end();
             ++it) {
            DeviceInfo dev = it->second;
            if (dev.target == target)
                vec.push_back(dev);
        }
        return vec;
    }

    std::map<std::string, DeviceInfo> getGroupDevicesMap() const
    {
        return m_groupDevicesMap;
    }

    DeviceInfo getGroupDevicesKey(std::string& key)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (m_groupDevicesMap.find(key) != m_groupDevicesMap.end())
            return m_groupDevicesMap[key];
        return DeviceInfo();
    }

    std::vector<DeviceInfo> getGroupDevicesTarget(std::string& target)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        std::vector<DeviceInfo> vec;
        for (std::map<std::string, DeviceInfo>::iterator it = m_groupDevicesMap.begin(); it != m_groupDevicesMap.end();
             ++it) {
            DeviceInfo dev = it->second;
            if (dev.target == target)
                vec.push_back(dev);
        }
        return vec;
    }

    User::Authority getAuthority() const
    {
        return m_currAuthority;
    }

    void setAuthority(User::Authority aty)
    {
        m_currAuthority = aty;
    }

    int getAlpha() const
    {
        return m_alpha;
    }

    int getBeta() const
    {
        return m_beta;
    }

    int getGamma() const
    {
        return m_gamma;
    }

    int getXp() const
    {
        return m_xp;
    }

    int getXs() const
    {
        return m_xs;
    }

    int getY() const
    {
        return m_y;
    }

    int getZ() const
    {
        return m_z;
    }

    double getTimeStep() const
    {
        return m_timeStep;
    }

    std::map<std::string, std::vector<float>> getAngleAndVelMap() const
    {

        return m_angleAndVelMap;
    }

    void getTargetAngleAndVel(const std::string& node, float& angle, float& vel)
    {
        std::unique_lock<std::mutex> lock(_mutex);
        if (m_angleAndVelMap.find(node) != m_angleAndVelMap.end()) {
            angle = m_angleAndVelMap[node][0];
            vel = m_angleAndVelMap[node][1];
        } else {
            throw std::runtime_error("Can not found " + node + " angle or velcoty.");
        }
    }

    std::string getLogPath() const
    {
        return m_logPath;
    }

    std::string getFlowCsvPath() const
    {
        return m_flowCsvPath;
    }

    std::string getSpecCsvPath() const
    {
        return m_specCsvPath;
    }

    std::string getPositionCsvPath() const
    {
        return m_positionCsvPath;
    }

    bool getMtcp() const
    {
        return m_Mtcp;
    }

    bool getMes() const
    {
        return m_Mes;
    }

    std::string getVersion() const
    {
        return m_version;
    }

    std::string getConfig() const
    {
        return m_config;
    }

    std::string getMtcpIP() const
    {
        return m_mtcpIP;
    }

    std::string getMtcpPort() const
    {
        return m_mtcpPort;
    }

    std::string getLotName() const
    {
        return m_lotName;
    }

    double getRms() const
    {
        return m_rms;
    }

    int getPid() const
    {
        return m_pid;
    }

private:
    void procError(const std::string& nodeStr);

    template <typename T>
    void setTestInfoTemplate(const std::string& node, const std::vector<T>& values)
    {
        XmlNode rootNode;
        XmlNode::ParseXml(m_pathOfUIConf, rootNode);
        XmlNode configurationNode = rootNode.Child(KConfiguration);
        if (configurationNode.isValid()) {
            XmlNode subRoot = configurationNode.Child(node);
            if (subRoot.isValid()) {
                std::vector<XmlNode> testInfos = subRoot.GetChilds("Value");
                int nsize = testInfos.size();
                int vNum = nsize > values.size() ? nsize : values.size();
                subRoot.SetAttribute("count", vNum);

                if (nsize == 0) {
                    for (int i = 0; i < values.size(); i++) {
                        subRoot.AddChild("Value", values[i]);
                    }
                } else {
                    for (int i = 0; i < values.size(); i++) {
                        if ((i + 1) > nsize) {
                            subRoot.AddChild("Value", values[i]);
                        } else {
                            XmlNode tNode = testInfos[i];
                            tNode.SetValue(values[i]);
                        }
                    }
                }
            } else {
                configurationNode.AddChild(node, "");
                subRoot = configurationNode.Child(node);

                subRoot.SetAttribute("count", (int)values.size());
                for (int i = 0; i < values.size(); i++) {
                    subRoot.AddChild("Value", values[i]);
                }
            }

        } else {
            procError(KConfiguration);
            return;
        }
        rootNode.Save(m_pathOfUIConf);
    }

private:
    std::mutex _mutex;
    int m_unitCount = 3;
    int m_offset = 0;
    int m_alpha = 36000;
    int m_beta = 500;
    int m_gamma = 500;
    int m_xp = 1250;
    int m_xs = 5000;
    int m_y = 5000;
    int m_z = 5000;
    int m_pid = -1;
    double m_timeStep = 0.0000016;
    double m_rms = 0.0;

    bool m_Mtcp = false;
    bool m_Mes = false;

    std::string m_slot = "";
    std::string m_machineID = "";
    std::string m_projectName = "";
    std::string m_version = "V1.0.0";
    std::string m_config = "";
    std::string m_logPath = "";
    std::string m_flowCsvPath = "";
    std::string m_specCsvPath = "";
    std::string m_positionCsvPath = "";
    std::string m_pathOfUIConf = "";
    std::string m_mtcpIP = "";
    std::string m_mtcpPort = "";
    std::string m_lotName = "";
    std::vector<bool> m_chooseVec;
    std::vector<std::string> m_groupOrderVec;
    std::map<std::string, DeviceInfo> m_unitDevicesMap;
    std::map<std::string, DeviceInfo> m_groupDevicesMap;
    std::map<std::string, std::vector<float>> m_angleAndVelMap;

    User::Authority m_currAuthority = User::Invalid;

    ConfigParse();
    Q_DISABLE_COPY(ConfigParse)
};
