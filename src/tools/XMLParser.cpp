#include "tools/XMLParser.hpp"
#include <libxml++/nodes/element.h>
#include <libxml++/parsers/domparser.h>
#include <vector>

xmlpp::Element *XMLParser::findChild(xmlpp::Element *parent, const std::string &name) {
    if (not parent) return nullptr;

    auto children = parent->get_children(name);
    for (auto *node : children) {
        if (auto *element = dynamic_cast<xmlpp::Element *>(node)) {
            return element;
        }
    }

    return nullptr;
}

std::vector<std::string> XMLParser::getAllVolumePaths() {
    std::vector<std::string> paths;

    xmlpp::Document *document = parser.get_document();
    xmlpp::Element *root = document->get_root_node();

    auto *devices = findChild(root, "devices");
    auto disks = devices->get_children();
    for (auto *diskNode : disks) {
        if (not diskNode) continue;
        auto *disk = dynamic_cast<xmlpp::Element *>(diskNode);

        auto *source = findChild(disk, "source");
        if (not source) continue;

        auto *file = source->get_attribute("file");
        if (not file) continue;

        std::string path = file->get_value();
        paths.push_back(path);
    }

    return paths;
}

std::string XMLParser::getDiskPath() {
    xmlpp::Document *document = parser.get_document();
    xmlpp::Element *root = document->get_root_node();

    auto *devices = findChild(root, "devices");
    auto *disk = findChild(devices, "disk");
    auto *source = findChild(disk, "source");
    auto *file = source->get_attribute("file");
    return file->get_value();
}

void XMLParser::setDiskPath(const std::string &path) {
    xmlpp::Document *document = parser.get_document();
    xmlpp::Element *root = document->get_root_node();

    auto *devices = findChild(root, "devices");
    auto *disk = findChild(devices, "disk");
    auto *source = findChild(disk, "source");
    auto *file = source->set_attribute("file", path);
}

void XMLParser::setName(const std::string &newName) {
    xmlpp::Document *document = parser.get_document();
    xmlpp::Element *domain = document->get_root_node();

    auto *name = findChild(domain, "name");
    name->set_child_text(newName);
}

void XMLParser::setUUID(const std::string &uuid) {
    xmlpp::Document *document = parser.get_document();
    xmlpp::Element *domain = document->get_root_node();

    auto *name = findChild(domain, "uuid");
    name->set_child_text(uuid);
}

void XMLParser::setMAC(const std::string &mac) {
    xmlpp::Document *document = parser.get_document();
    xmlpp::Element *domain = document->get_root_node();

    auto *devices = findChild(domain, "devices");
    auto *interface = findChild(devices, "interface");
    auto *MAC = findChild(interface, "mac");
    MAC->set_attribute("address", mac);
}

std::string XMLParser::getXML() {
    xmlpp::Document *document = parser.get_document();
    return document->write_to_string();
}
