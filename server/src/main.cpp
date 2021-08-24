
//
#include "main.hpp"

int main()
{
    std::vector<std::string> msg {"Hello", "C++", "World", "from", "VS Code", "and the C++ extension!"};

    for (const std::string& word : msg)
    {
        std::cout << word << " ";
    }
    std::cout << std::endl;

    boost::property_tree::ptree pt;
    try {
        
        boost::property_tree::read_json("../resources/data.json", pt);

        const long double latitude = pt.get<long double>("point.latitude", 0);

        const long double longitude = pt.get<long double>("point.longitude");

        std::cout << "Le point est : " << std::endl << "Latitude : " << latitude << std::endl << "Longitude : " << longitude << std::endl;
    } catch (const boost::wrapexcept<boost::property_tree::json_parser::json_parser_error>& ex) {
        // ERROR TYPE : ESTRUCTOBJ, object has bad JSON structure
        // ERROR TYPE : EREADOBJ, IO
        // ERROR TYPE : EUNKOBJ unknow error on object
        std::smatch m;
        const std::string errorStr(ex.what());
        // filename, number line error, error message
        std::regex re("(.+)\\((\\d+)\\): (.*)");
        std::regex_search (errorStr, m, re);
        if (!m.size()) {
            re = std::regex ("(.+): (.*)");
            std::regex_search (errorStr, m, re);
        }
        if (m.size() == 4) {
        // example : "data.json(5): garbage after data"
            std::cerr << "Le fichier " << m[1] << " n'est pas un JSON valide, ligne " << m[2] << " : " << m[3] << std::endl;
        } else if (m.size() == 3) {
            // example : data.json: cannot open file
            std::cerr << "Le fichier " << m[1] << " n'est pas un JSON valide : " << m[2] << std::endl;
        } else {
            std::cerr << "Erreur de parsing du fichier " << ex.what() << std::endl;
        }
    } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_path>& ex) {
        // ERROR TYPE : ENOKEY, mandatory key not found
        // FIELD : [NAME, ERROR]
        std::cerr << "data.json ne contient pas un attribut obligatoire : " << ex.what() << std::endl;
    } catch (const boost::wrapexcept<boost::property_tree::ptree_bad_data>& ex) {
        // ERROR TYPE : EVALUETYPE, key found with bad value type
        // FIELD : [NAME, ERROR]
        std::cerr << "data.json contient une données de type invalide : " << ex.what() << std::endl;
    }
}