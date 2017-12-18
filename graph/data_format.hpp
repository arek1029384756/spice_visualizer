#ifndef DATA_FORMAT
#define DATA_FORMAT

#include <string>
#include <list>

namespace graph {

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

            const std::string& getConnection() const {
                return m_connection;
            }
        };

        std::string m_type;
        std::string m_name;
        std::string m_value;
        std::list<Terminal> m_terminals;

        public:
        Component(const std::string& type, const std::string& name, const std::string& value, const std::list<std::string>& connections)
            : m_type(type), m_name(name), m_value(value) {

            for(auto conn : connections) {
                Terminal terminal("tmp", conn);
                m_terminals.emplace_back(terminal);
            }
        }

        const std::string& getName() const {
            return m_name;
        }

        void getConnections(std::list<std::string>& connections) const {
            for(const auto& term : m_terminals) {
                connections.emplace_back(term.getConnection());
            }
        }
    };


    class CircuitGraph {

        CircuitGraph() {}
        CircuitGraph(const CircuitGraph&) = delete;

        std::map<std::string, Component> m_componentMap;
        std::map<std::string, Connection> m_connectionMap;

        void addConnection(const std::string& name) {
            m_connectionMap.emplace(name, Connection(name));
        }

        public:
        static CircuitGraph& getInstance() {
            static CircuitGraph instance;
            return instance;
        }

        Connection& updateConnection(const std::string& name) {
            auto conn = m_connectionMap.find(name);

            if(conn == m_connectionMap.end()) {
                addConnection(name);
                conn = m_connectionMap.find(name);
            }
            return conn->second;
        }

        const Connection& getConnection(const std::string& name) const {
            auto conn = m_connectionMap.find(name);
            if(conn == m_connectionMap.end()) {
                throw std::runtime_error(std::string("Connection '") +
                        name + std::string("' not found!"));
            }
            return conn->second;
        }

        const Component& getComponent(const std::string& name) const {
            auto comp = m_componentMap.find(name);
            if(comp == m_componentMap.end()) {
                throw std::runtime_error(std::string("Component '") +
                        name + std::string("' not found!"));
            }
            return comp->second;
        }

        void addComponent(const std::string& type, const std::string& name, const std::string& value, const std::list<std::string>& connections) {
            auto comp = Component(type, name, value, connections);
            m_componentMap.emplace(name, comp);

            for(const auto& connName : connections) {
                auto& conn = updateConnection(connName);
                conn.attachComponent(name);
            }
        }

        void print() const {
            for(const auto& compPair : m_componentMap) {
                const auto& compName = compPair.first;
                const auto& component = compPair.second;
                std::cout << std::endl << "For component: " << compName << std::endl;
                std::list<std::string> cons;
                component.getConnections(cons);
                for(const auto& connName : cons) {
                    std::cout << "   conname: " << connName << std::endl;
                    const auto& conn = getConnection(connName);
                    for(const auto& comp : conn.getComponents()) {
                        std::cout << "      compname: " << comp << std::endl;
                    }
                }
            }
        }

    };


}

#endif
