#include "scaler/VMCloner.hpp"
#include "Constants.hpp"
#include "VM.hpp"
#include "logger/Logger.hpp"
#include "tools/XMLParser.hpp"
#include <filesystem>
#include <libvirt/libvirt.h>
#include <libxml++/document.h>
#include <libxml++/libxml++.h>
#include <libxml++/nodes/element.h>
#include <libxml++/parsers/domparser.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <stdexcept>
#include <string>

std::string VMCloner::buildNewDiskPath(const std::string &sourceVMDiskPath, const std::string &cloneVMName) const {
    std::filesystem::path p(sourceVMDiskPath);
    std::filesystem::path parentPath = p.parent_path();
    std::filesystem::path newPath = parentPath / (cloneVMName + ".qcow2");

    return newPath.string();
}

std::string VMCloner::buildStorageVolXML(const std::string &diskImageFileName, unsigned long long capacity) const {
    return "<volume>"
           "<name>" +
           diskImageFileName +
           "</name>"
           "<capacity unit='bytes'>" +
           std::to_string(capacity) +
           "</capacity>"
           "<target>"
           "<format type='qcow2'/>"
           "</target>"
           "</volume>";
}

void VMCloner::cloneDisk(const std::string &sourceVMDiskImageFilePath, const std::string &cloneVMDiskImageFileName) const {
    LOG_INFO("Cloning Disk from {} to {}", sourceVMDiskImageFilePath, cloneVMDiskImageFileName);
    virStorageVolPtr sourceVolume = virStorageVolLookupByPath(connection, sourceVMDiskImageFilePath.c_str());
    if (not sourceVolume) {
        throw std::runtime_error("Failed to get Source Volume");
    }

    virStoragePoolPtr sourcePool = virStoragePoolLookupByName(connection, Constants::DEFAULT_STORAGE_POOL_NAME);
    if (not sourcePool) {
        throw std::runtime_error("Failed to get Default Source Pool");
    }

    virStorageVolInfo volInfo{};
    if (virStorageVolGetInfo(sourceVolume, &volInfo) < 0) {
        virStoragePoolFree(sourcePool);
        virStorageVolFree(sourceVolume);
        throw std::runtime_error("Failed to get Storage Volume Info");
    }

    std::string xml = buildStorageVolXML(cloneVMDiskImageFileName, volInfo.capacity);
    virStorageVolPtr cloneVolume = virStorageVolCreateXMLFrom(sourcePool, xml.c_str(), sourceVolume, 0);
    if (not cloneVolume) {
        virStoragePoolFree(sourcePool);
        virStorageVolFree(sourceVolume);
        throw std::runtime_error("Failed to clone Storage Volume");
    }

    virStoragePoolFree(sourcePool);
    virStorageVolFree(sourceVolume);
    virStorageVolFree(cloneVolume);
}

VM VMCloner::clone(const std::string &sourceVMName, const std::string &cloneVMName) const {
    LOG_INFO("Cloning VM");
    virDomainPtr sourceDomain = virDomainLookupByName(connection, sourceVMName.c_str());
    char *xml = virDomainGetXMLDesc(sourceDomain, VIR_DOMAIN_XML_INACTIVE);

    XMLParser parser(xml);
    free(xml);

    std::string sourceVMDiskPath = parser.getDiskPath();
    std::string newVMDiskPath = buildNewDiskPath(sourceVMDiskPath, cloneVMName);

    LOG_DEBUG("\tUpdating Name, UUID, MAC and DiskPath of cloned VM");
    parser.setName(cloneVMName);
    parser.setUUID(idGenerator.generateUUID());
    parser.setMAC(idGenerator.generateMAC());
    parser.setDiskPath(newVMDiskPath);

    std::string cloneVMDiskName = cloneVMName + Constants::DISK_IMAGE_FILE_EXTENSION;
    cloneDisk(sourceVMDiskPath, cloneVMDiskName);

    LOG_DEBUG("\tDefining VM XML Config");
    std::string clonedVMXML = parser.getXML();
    virDomainPtr cloneVMPtr = virDomainDefineXML(connection, clonedVMXML.c_str());

    virDomainFree(sourceDomain);
    return VM(cloneVMPtr, cloneVMName);
}
