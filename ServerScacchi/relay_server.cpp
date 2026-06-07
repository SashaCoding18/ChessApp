#include <SFML/Network.hpp>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <vector>

sf::Packet& operator <<(sf::Packet& packet, const std::vector<signed char>& data) {
    packet << static_cast<std::uint64_t>(data.size()); 
    
    for (signed char value : data) {
        packet << value;
    }
    return packet;
    }
sf::Packet& operator>>(sf::Packet& packet, std::vector<signed char>& data) {
    std::uint64_t size;
    if (packet >> size) {
        data.clear();
        data.reserve(size);
        for (std::uint64_t i = 0; i < size; ++i) {
            signed char value;
            if (packet >> value) {
                data.push_back(value);
            }
        }
    }
    return packet;
}

int gestione_partita(sf::TcpSocket p1, sf::TcpSocket p2){
    sf::Packet packet;
    sf::Packet invio;
    std::vector<signed char> xx;
    int disconnesso=0;
    int n=rand()%2;
    packet<<n;
    p1.send(packet);
    packet.clear();
    if(n==1){n=0;}else{n=1;}
    packet<<n;
    p2.send(packet);
    p1.setBlocking(false);
    p2.setBlocking(false);
    while (true) {
        // Se P1 manda una mossa, la mando a P2
        sf::Socket::Status p1Status=p1.receive(packet);
        if (p1Status == sf::Socket::Status::Done) {
            packet>>xx;
            invio.clear();
            invio<<1;
            invio<<xx;
            p2.send(invio);
            std::cout<<"rec"<<std::endl;
            packet.clear();
        }else if(p1Status == sf::Socket::Status::Disconnected||p1Status==sf::Socket::Status::Error){
            disconnesso=1;
            break;
        }
        // Se P2 manda una mossa, la mando a P1
        sf::Socket::Status p2Status=p2.receive(packet);
        if (p2Status  == sf::Socket::Status::Done) {
            invio.clear();
            packet>>xx;
            invio<<1;
            invio<<xx;
            p1.send(invio);
            std::cout<<"rec"<<std::endl;
            packet.clear();
        }else if(p2Status==sf::Socket::Status::Disconnected||p2Status==sf::Socket::Status::Error){
            disconnesso=2;
            break;
        }
        sf::sleep(sf::milliseconds(10)); // Evita di consumare troppa CPU
    }
    if(disconnesso==1){
        std::cout<<"client 1 disconnesso"<<std::endl;
        invio.clear();
        invio<<2;
        p2.send(invio);
    }else if(disconnesso==2){
        std::cout<<"client 2 disconnesso"<<std::endl;
        invio.clear();
        invio<<2;
        p1.send(invio);
    }
    p1.disconnect();
    p2.disconnect();
    return 0;
}

int main() {
    sf::TcpListener listener;
    std::srand(time(NULL));

    if (listener.listen(53000) != sf::Socket::Status::Done) {
        std::cerr << "Errore nell'avvio del server!" << std::endl;
        return -1;
    }
    listener.setBlocking(false);

    std::cout << "Relay Server pronto. In attesa di giocatori..." << std::endl;
    sf::TcpSocket* inAttesa = nullptr;
    sf::Packet testPacket;

    while (true) {
        sf::TcpSocket* nuovoClient = new sf::TcpSocket();
        nuovoClient->setBlocking(false);

        sf::Socket::Status status = listener.accept(*nuovoClient);

        if (status == sf::Socket::Status::Done) {
            std::cout << "Nuovo client rilevato a livello di rete." << std::endl;

            if (inAttesa == nullptr) {
                inAttesa = nuovoClient;
                std::cout << "Giocatore 1 connesso! In attesa di un avversario..." << std::endl;
            } else {
                std::cout << "Giocatore 2 connesso! Avvio della partita." << std::endl;
                std::thread newGame(gestione_partita, std::move(*inAttesa), std::move(*nuovoClient));
                newGame.detach();
                delete inAttesa;
                delete nuovoClient;
                inAttesa = nullptr;
            }
        } else {
            delete nuovoClient;
        }
        if (inAttesa != nullptr) {
            sf::Socket::Status check = inAttesa->receive(testPacket);
            
            if (check == sf::Socket::Status::Disconnected || check == sf::Socket::Status::Error) {
                std::cout << "Il Giocatore 1 ha abbandonato la ricerca o chiuso l'app. Svuoto la coda." << std::endl;
                inAttesa->disconnect();
                delete inAttesa;
                inAttesa = nullptr;
            }
        }
        sf::sleep(sf::milliseconds(10));
    }

    return 0;
}