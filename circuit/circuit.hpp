#ifndef CIRCUIT
#define CIRCUIT

#include <string>
#include <list>
#include <set>
#include <initializer_list>

namespace circuit {

    class Recommendation {
        static const std::list<Recommendation> priority;

        std::string m_name;
        std::string m_referenceTerminal;

        public:
        Recommendation()
            : m_name(""), m_referenceTerminal("") {}

        Recommendation(const std::string& name)
            : m_name(name), m_referenceTerminal("") {
        }

        virtual ~Recommendation() = default;

        bool operator==(const Recommendation& other) const {
            return m_name == other.getName();
        }

        bool operator!=(const Recommendation& other) const {
            return !operator==(other);
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

        void setReferenceTerminal(const std::string& termName) {
            m_referenceTerminal = termName;
        }

        const std::string& getReferenceTerminal() const {
            return m_referenceTerminal;
        }
    };
    const std::list<Recommendation> Recommendation::priority = {
                                                                    Recommendation("UP"),
                                                                    Recommendation("LEFT"),
                                                                    Recommendation("RIGHT"),
                                                                    Recommendation("DOWN")
                                                                };


    class Connection {
        std::string m_name;
        std::list<std::string> m_components;

        public:
        Connection(const std::string& name)
            : m_name(name) {}

        virtual ~Connection() = default;

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
            : m_type(type), m_name(name), m_value(value), m_recommendation(Recommendation()) {

            std::size_t i = 0;
            for(auto connName : connections) {
                Terminal terminal(std::to_string(++i), connName);
                m_terminals.emplace_back(terminal);
            }
        }

        virtual ~Component() = default;

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

        const std::list<Terminal>& getTerminals() const {
            return m_terminals;
        }

        const Recommendation& getRecommendation() const {
            return m_recommendation;
        }

        void setRecommendation(const Recommendation& recomm) {
            m_recommendation = recomm;
        }
    };


    template<typename TSchematicInterface, typename TProgressInterface>
    class CircuitGraph {

        typedef std::set<Recommendation, std::greater<Recommendation>> RecommPrioritySet;

        CircuitGraph(const CircuitGraph&) = delete;
        CircuitGraph& operator=(const CircuitGraph&) = delete;

        std::map<std::string, Recommendation> m_circuitTerminals;

        std::map<std::string, Component> m_componentMap;
        std::map<std::string, Connection> m_connectionMap;

        TSchematicInterface* m_schIfc;
        TProgressInterface* m_progressIfc;

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
                throw std::runtime_error(std::string("Object of type '") + typeid(T).name() + std::string("' and name '") +
                        name + std::string("' not found!"));
            }
            return iter->second;
        }

        const Component& getCoreComponent() const {
            //Temporary implementation
            for(const auto& compPair : m_componentMap) {
                const auto& compType = compPair.second.getType();
                if(compType == "Q" || compType == "X") {
                    return compPair.second;
                }
            }

            throw std::runtime_error("Core component not found!");
        }

        Recommendation getPriorityRecommendation(const RecommPrioritySet& recommendations) const {
            if(!recommendations.empty()) {
                auto it = recommendations.begin();
                std::cout << it->getName() << std::endl;
                return *it;
            } else {
                return Recommendation();
            }
        }

        Recommendation getConnectionRecommendation(const std::string& connName) const {
            auto it = m_circuitTerminals.find(connName);
            if(it == m_circuitTerminals.end()) {
                throw std::runtime_error("");
            }
            return it->second;
        }

        Recommendation connectionTraversal(const std::string& connName,
                                            const std::set<std::string>& compPath,
                                            const std::set<std::string>& connPath) {
            try {
                return getConnectionRecommendation(connName);
            } catch(const std::runtime_error& e) {
                //No terminal recommendation found, continue...
            }

            auto& connObj = getConnectionObject(connName);
            auto& components = connObj.getComponents();

            std::set<std::string> connPathNext = connPath;
            connPathNext.emplace(connName);

            RecommPrioritySet recommendations;
            for(const auto& compName : components) {
                if(compPath.find(compName) == compPath.end()) {
                    auto recomm = componentTraversal(compName, compPath, connPathNext);
                    recommendations.emplace(recomm);
                }
            }

            return getPriorityRecommendation(recommendations);
        }

        Recommendation componentTraversal(const std::string& compName,
                                            const std::set<std::string>& compPath,
                                            const std::set<std::string>& connPath) {

            auto& compObj = getComponentObject(compName);
            const auto& terminals = compObj.getTerminals();

            std::set<std::string> compPathNext = compPath;
            compPathNext.emplace(compName);

            Recommendation recomm;
            for(const auto& term : terminals) {
                const auto& connName = term.getConnection();
                if(connPath.find(connName) == connPath.end()) {
                    recomm = connectionTraversal(connName, compPathNext, connPath);
                    recomm.setReferenceTerminal(term.getName());
                }
            }

            if(recomm != Recommendation()) {
                auto& comp = const_cast<Component&>(compObj);
                comp.setRecommendation(recomm);
            }

            return recomm;
        }

        public:
        CircuitGraph(TSchematicInterface * const schIfc, TProgressInterface * const progressIfc)
            : m_schIfc(schIfc), m_progressIfc(progressIfc) {
        }

        virtual ~CircuitGraph() = default;

        template<typename T = std::string>
        void setTerminals(const std::initializer_list<T> initL) {
            static const std::size_t N = 4;
            auto size = initL.size();
            if(size != N) {
                throw std::runtime_error(std::string("Wrong number of terminals (") +
                        std::to_string(size) + std::string(" != ") + std::to_string(N) + std::string(")!"));
            } else {
                auto it = initL.begin();
                m_circuitTerminals = {  {*std::next(it, 0), Recommendation("UP")},
                                        {*std::next(it, 1), Recommendation("DOWN")},
                                        {*std::next(it, 2), Recommendation("LEFT")},
                                        {*std::next(it, 3), Recommendation("RIGHT")}
                                    };
            }
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

            componentTraversal(coreComp.getName(), {}, {});
        }

        void print() const {
            for(const auto& compPair : m_componentMap) {
                const auto& rootCompName = compPair.first;
                const auto& rootComp = compPair.second;

                std::cout << std::endl << "For component: " << rootCompName << std::endl;
                const auto& terminals = rootComp.getTerminals();
                for(const auto& term : terminals) {
                    std::cout << "    term: " << term.getName() << " conname: " << term.getConnection() << std::endl;
                    const auto& connObj = getConnectionObject(term.getConnection());
                    for(const auto& compName : connObj.getComponents()) {
                        const auto& compObj = getComponentObject(compName);
                        std::cout << "      compname: " << compObj.getName()
                            << "\t(" << compObj.getValue() << ")" << std::endl;
                    }
                }
            }
        }

        void printRecommendations() const {
            for(const auto& compPair : m_componentMap) {
                const auto& rootCompName = compPair.first;
                const auto& rootComp = compPair.second;
                const auto& recomm = rootComp.getRecommendation();

                std::cout << std::endl
                          << "Component: "
                          << rootCompName
                          << "\tR:"
                          << recomm.getName()
                          << "\tRefT:"
                          << recomm.getReferenceTerminal()
                          << std::endl;
            }
        }


    };

}

#endif
