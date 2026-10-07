#ifndef XML_PARSER_HPP
#define XML_PARSER_HPP

#include <libvirt/libvirt.h>
#include <libxml++/nodes/element.h>
#include <libxml++/parsers/domparser.h>
#include <string>
#include <vector>

class XMLParser {
  private:
    xmlpp::DomParser parser;

    xmlpp::Element *findChild(xmlpp::Element *parent, const std::string &name);

  public:
    XMLParser(char *xml) {
        parser.parse_memory(xml);
    };

    std::string getDiskPath();
    std::vector<std::string> getAllVolumePaths();

    void setName(const std::string &newName);
    void setUUID(const std::string &uuid);
    void setMAC(const std::string &mac);
    void setDiskPath(const std::string &path);

    std::string getXML();
};

#endif
