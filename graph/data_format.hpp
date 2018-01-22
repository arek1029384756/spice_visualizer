#ifndef DATA_FORMAT
#define DATA_FORMAT

#include <string>
#include <list>
#include <set>

namespace graph {

    class Recommendation {
        std::string m_name;

        Recommendation() = delete;

        public:
        static const std::list<Recommendation> priority;

        Recommendation(const std::string& name)
            : m_name(name) {
        }

        bool operator==(const Recommendation& other) const {
            return m_name == other.getName();
        }

        bool operator<(const Recommendation& other) const {
            if(*this == other) {
                return false;
            }

            for(const auto& r : Recommendation::priority) {
                if(r.getName() == m_name) {
                    return false;
                }
                if(r.getName() == other.getName()) {
                    return true;
                }
            }
            return false;
        }

        bool operator>(const Recommendation& other) const {
            return !(*this == other) && !(*this < other);
        }

        const std::string& getName() const {
            return m_name;
        }
    };
    const std::list<Recommendation> Recommendation::priority = { Recommendation("UP"), Recommendation("DOWN"), Recommendation("LEFT"), Recommendation("RIGHT") };
    typedef std::set<Recommendation, std::greater<Recommendation>> RecommPrioritySet;

    class Connection {
        std::string m_name;
        std::list<std::string> m_components;

        public:
        Connection(const std::string& name)
            : m_name(name) {}

        void attachComponent(const std::string& name) {
            m_components.emplace_back(name);
        }

        const std::string& getName() const {
            return m_name;
        }

        const std::list<std::string>& getComponents() const {
            return m_components;
        }
    };


    class Component {
        class Terminal {
            std::string m_name;
            std::string m_connection;

            public:
            Terminal(const std::string& name, const std::string& connection)
                : m_name(name), m_connection(connection) {}

            const std::string& getName() const {
                return m_name;
            }

            const std::string& getConnection() const {
                return m_connection;
            }
        };

        std::string m_type;
        std::string m_name;
        std::string m_value;
        Recommendation m_recommendation;
        std::list<Terminal> m_terminals;

        public:
        Component(const std::string& type,
                const std::string& name,
                const std::string& value,
                const std::list<std::string>& connections)
            : m_type(type), m_name(name), m_value(value), m_recommendation(Recommendation("")) {

            for(auto connName : connections) {
                Terminal terminal("tmp", connName);
                m_terminals.emplace_back(terminal);
            }
        }

        const std::string& getType() const {
            return m_type;
        }

        const std::string& getName() const {
            return m_name;
        }

        const std::string& getValue() const {
            return m_value;
        }

        void getConnections(std::list<std::string>& connections) const {
            for(const auto& term : m_terminals) {
                connections.emplace_back(term.getConnection());
            }
        }

        const Recommendation& getRecommendation() const {
            return m_recommendation;
        }

        void setRecommendation(const Recommendation& recomm) {
            m_recommendation = recomm;
        }
    };


    class CircuitGraph {

        CircuitGraph() {}
        CircuitGraph(const CircuitGraph&) = delete;
        CircuitGraph& operator=(const CircuitGraph&) = delete;

        std::map<std::string, Component> m_componentMap;
        std::map<std::string, Connection> m_connectionMap;


        void addConnection(const std::string& name) {
            m_connectionMap.emplace(name, Connection(name));
        }

        Connection& updateConnection(const std::string& name) {
            auto connIter = m_connectionMap.find(name);

            if(connIter == m_connectionMap.end()) {
                addConnection(name);
                connIter = m_connectionMap.find(name);
                if(connIter == m_connectionMap.end()) {
                    throw std::runtime_error(std::string("Failed to update connection '") +
                            name + std::string("'!"));
                }
            }
            return connIter->second;
        }

        template<typename T>
        const T& getObject(const std::map<std::string, T>& table, const std::string& name) const {
            auto iter = table.find(name);
            if(iter == table.end()) {
                throw std::runtime_error(std::string(typeid(T).name()) + std::string(" '") +
                        name + std::string("' not found!"));
            }
            return iter->second;
        }

        const Component& getCoreComponent() const {
            //Temporary implementation
            std::string coreName = "Qnpn";
            auto compIter = m_componentMap.find(coreName);
            if(compIter == m_componentMap.end()) {
                throw std::runtime_error(std::string("Core component '") + coreName + std::string("' not found!"));
            } else {
                return compIter->second;
            }
        }

        Recommendation getPriorityRecommendation(const RecommPrioritySet& recommendations) {
            if(!recommendations.empty()) {
                auto it = recommendations.begin();
                std::cout << it->getName() << std::endl;
                return *it;
            } else {
                throw std::runtime_error(std::string("Empty recommendations set passed!"));
            }
        }

        Recommendation connectionTraversal(const std::string& connName, const std::string& rootComponent) {
            if(connName == "vdd") {
                return Recommendation("UP");
            } else if(connName == "vss") {
                return Recommendation("DOWN");
            } else if(connName == "gen") {
                return Recommendation("LEFT");
            } else if(connName == "out") {
                return Recommendation("RIGHT");
            }

            auto& connObj = getConnectionObject(connName);
            auto& components = connObj.getComponents();
            RecommPrioritySet recommendations;
            for(const auto& compName : components) {
                if(compName != rootComponent) {
                    auto recomm = componentTraversal(compName, connName);
                    recommendations.emplace(recomm);
                }
            }

            return getPriorityRecommendation(recommendations);
        }

        Recommendation componentTraversal(const std::string& compName, const std::string& rootConnection) {
            auto& compObj = getComponentObject(compName);
            std::list<std::string> connections;
            compObj.getConnections(connections);

            Recommendation recomm("");
            for(const auto& connName : connections) {
                if(connName != rootConnection) {
                    recomm = connectionTraversal(connName, compName);
                }
            }

            auto& comp = const_cast<Component&>(compObj);
            comp.setRecommendation(recomm);

            return recomm;
        }

        public:
        static CircuitGraph& getInstance() {
            static CircuitGraph instance;
            return instance;
        }

        const Connection& getConnectionObject(const std::string& name) const {
            return getObject<Connection>(m_connectionMap, name);
        }

        const Component& getComponentObject(const std::string& name) const {
            return getObject<Component>(m_componentMap, name);
        }

        void addComponent(const std::string& type,
                const std::string& name,
                const std::string& value,
                const std::list<std::string>& connections) {
            auto compObj = Component(type, name, value, connections);
            m_componentMap.emplace(name, compObj);

            for(const auto& connName : connections) {
                auto& connObj = updateConnection(connName);
                connObj.attachComponent(name);
            }
        }

        void createRecomendations() {
            auto& coreComp = getCoreComponent();
            std::cout << coreComp.getType() << "  " << coreComp.getName() << "  " << coreComp.getValue()  << std::endl;

            componentTraversal(coreComp.getName(), "");
        }

        void print() const {
            for(const auto& compPair : m_componentMap) {
                const auto& rootCompName = compPair.first;
                const auto& rootComp = compPair.second;

                std::cout << std::endl << "For component: " << rootCompName << std::endl;
                std::list<std::string> rootCons;
                rootComp.getConnections(rootCons);
                for(const auto& connName : rootCons) {
                    std::cout << "   conname: " << connName << std::endl;
                    const auto& connObj = getConnectionObject(connName);
                    for(const auto& compName : connObj.getComponents()) {
                        const auto& compObj = getComponentObject(compName);
                        std::cout << "      compname: " << compObj.getName()
                            << "\t(" << compObj.getValue() << ")" << std::endl;
                    }
                }
            }
        }

        void printiRecommendations() const {
            for(const auto& compPair : m_componentMap) {
                const auto& rootCompName = compPair.first;
                const auto& rootComp = compPair.second;

                std::cout << std::endl << "Component: " << rootCompName << "\tR:" << rootComp.getRecommendation().getName() << std::endl;
            }
        }


    };


}

#endif
