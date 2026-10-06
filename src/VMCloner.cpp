#include "VMCloner.hpp"
#include "VM.hpp"
#include "XMLParser.hpp"
#include "utils/RandomGenerator.hpp"
#include <filesystem>
#include <iostream>
#include <libvirt/libvirt.h>
#include <libxml++/document.h>
#include <libxml++/libxml++.h>
#include <libxml++/nodes/element.h>
#include <libxml++/parsers/domparser.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <ostream>
#include <sstream>
#include <stdexcept>
#include <string>

std::string VMCloner::generateMAC() const {
    std::stringstream ss;
    ss << "52:54:00:";

    for (int i = 0; i < 3; i++) {
        int randomNum = RandomGenerator::generate(0, 15);
        ss << std::hex << randomNum;
        randomNum = RandomGenerator::generate(0, 15);
        ss << std::hex << randomNum << std::dec;

        if (i != 2) ss << ":";
    }

    std::cout << "Generated MAC: " << ss.str() << std::endl;
    return ss.str();
}

std::string VMCloner::generateUUID() const {
    std::stringstream ss;
    const int groups[] = {8, 4, 4, 4, 12};

    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < groups[i]; j++) {
            int randomNum = RandomGenerator::generate(0, 15);
            ss << std::hex << randomNum << std::dec;
        }

        if (i != 4) {
            ss << "-";
        }
    }

    std::cout << "Generated UUID: " << ss.str() << std::endl;
    return ss.str();
}

std::string VMCloner::generateNewDiskPath(const std::string &sourceVMDiskPath, const std::string &cloneVMName) const {
    std::filesystem::path p(sourceVMDiskPath);
    std::filesystem::path parentPath = p.parent_path();
    std::filesystem::path newPath = parentPath / (cloneVMName + ".qcow2");

    return newPath.string();
}

void VMCloner::cloneDisk(const std::string &sourceVMDiskPath, const std::string &cloneVMDiskName) const {
    virStorageVolPtr sourceVolume = virStorageVolLookupByPath(connection, sourceVMDiskPath.c_str());
    if (not sourceVolume) {
        throw std::runtime_error("Failed to get Source Volume");
    }

    virStoragePoolPtr sourcePool = virStoragePoolLookupByName(connection, "default");
    if (not sourcePool) {
        throw std::runtime_error("Failed to get Default Source Pool");
    }

    virStorageVolInfo volInfo{};
    if (virStorageVolGetInfo(sourceVolume, &volInfo) < 0) {
        virStoragePoolFree(sourcePool);
        virStorageVolFree(sourceVolume);

        throw std::runtime_error("Failed to get Storage Volume Info");
    }

    std::string xml = "<volume>"
                      "<name>" +
                      cloneVMDiskName +
                      "</name>"
                      "<capacity unit='bytes'>" +
                      std::to_string(volInfo.capacity) +
                      "</capacity>"
                      "<target>"
                      "<format type='qcow2'/>"
                      "</target>"
                      "</volume>";

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
    virDomainPtr sourceDomain = virDomainLookupByName(connection, sourceVMName.c_str());
    char *xml = virDomainGetXMLDesc(sourceDomain, VIR_DOMAIN_XML_INACTIVE);

    XMLParser parser(xml);
    free(xml);

    std::string sourceVMDiskPath = parser.getDiskPath();
    std::string newVMDiskPath = generateNewDiskPath(sourceVMDiskPath, cloneVMName);

    parser.setName(cloneVMName);
    parser.setUUID(generateUUID());
    parser.setMAC(generateMAC());
    parser.setDiskPath(newVMDiskPath);

    std::string clonedVMXML = parser.getXML();

    std::string cloneVMDiskName = cloneVMName + ".qcow2";
    cloneDisk(sourceVMDiskPath, cloneVMDiskName);

    virDomainPtr newDomainPtr = virDomainDefineXML(connection, clonedVMXML.c_str());

    virDomainFree(sourceDomain);
    return VM(newDomainPtr);
}
