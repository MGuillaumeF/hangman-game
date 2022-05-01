boost::property_tree::ptree pt;
  try {

    boost::property_tree::read_json("../resources/data.json", pt);

    const auto latitude = pt.get<std::double_t>("point.latitude", 0);

    const auto longitude = pt.get<std::double_t>("point.longitude");

    logger->info("HTTP_CONFIGURATION",
                 "Le point est : \r\n Latitude : " + std::to_string(latitude) +
                     "\r\n Longitude : " + std::to_string(longitude));
  } catch (const boost::wrapexcept<
           boost::property_tree::json_parser::json_parser_error> &ex) {
    // ERROR TYPE : ESTRUCTOBJ, object has bad JSON structure
    // ERROR TYPE : EREADOBJ, IO
    // ERROR TYPE : EUNKOBJ unknow error on object
    std::smatch m;
    const std::string errorStr(ex.what());
    // filename, number line error, error message
    std::regex re(R"(.+)\((\d+)\): (.*)");
    std::regex_search(errorStr, m, re);
    if (m.empty()) {
      re = std::regex("(.+): (.*)");
      std::regex_search(errorStr, m, re);
    }
    const uint8_t jsonStructErrorSize = 4;
    const uint8_t accessFileErrorSize = 3;
    if (jsonStructErrorSize == m.size()) {
      // example : "data.json(5): garbage after data"
      const uint8_t filenameIndex = 1;
      const uint8_t lineIndex = 2;
      const uint8_t messageIndex = 3;
      logger->error("HTTP_CONFIGURATION",
                    "Le fichier " + m[filenameIndex].str() +
                        " n'est pas un JSON valide, ligne " +
                        m[lineIndex].str() + " : " + m[messageIndex].str());
    } else if (accessFileErrorSize == m.size()) {
      // example : data.json: cannot open file
      const uint8_t filenameIndex = 1;
      const uint8_t messageIndex = 2;
      logger->error("HTTP_CONFIGURATION",
                    "Le fichier " + m[filenameIndex].str() +
                        " n'est pas un JSON valide : " + m[messageIndex].str());
    } else {
      logger->error("HTTP_CONFIGURATION",
                    "Erreur de parsing du fichier " + std::string(ex.what()));
    }
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_path> &ex) {
    // ERROR TYPE : ENOKEY, mandatory key not found
    // FIELD : [NAME, ERROR]
    logger->error("HTTP_CONFIGURATION",
                  "data.json ne contient pas un attribut obligatoire : " +
                      std::string(ex.what()));
  } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data> &ex) {
    // ERROR TYPE : EVALUETYPE, key found with bad value type
    // FIELD : [NAME, ERROR]
    logger->error("HTTP_CONFIGURATION",
                  "data.json contient une données de type invalide : " +
                      std::string(ex.what()));
  }
