#include<SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include "Hmain.hpp"
#include <vector>
#include <algorithm>
#include <cstdint>
#include <cmath>
using namespace std;



int main(){
    sf::TcpSocket socket;
    sf::Packet packet;
    auto serverIp = sf::IpAddress::resolve("127.0.0.1").value();
    unsigned short port = 53000;
    window.setView(view);
    window.setFramerateLimit(30);
    Button LocalB(Lbutton,0.8f,0.8f,640.f,243.f);
    Button OnlineB(Obutton,0.8f,0.8f,640.f,457.f);
    Button Impostazioni(ex,0.5f,0.5f,1200.f,50.f);
    Button matchmaking(BG,1.f,1.f,640.f,360.f);
    Button Cancel(x,0.2f,0.2f,640.f,450.f);
    sf::Clock animationClock;
    sf::Color neonCyan(0, 255, 255);
    sf::Text textStatic(font, "SEARCHING FOR OPPONENT", 22);
    textStatic.setFillColor(neonCyan);
    sf::Text dot1(font, ".", 30); dot1.setFillColor(neonCyan);
    sf::Text dot2(font, ".", 30); dot2.setFillColor(neonCyan);
    sf::Text dot3(font, ".", 30); dot3.setFillColor(neonCyan);
    bool mouseClicked = false;
    //setSprites();
    while(window.isOpen()){   
        mouseClicked = false;
        while(std::optional event = window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    window.close();
                }
                if (const auto* mousePressed = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mousePressed->button == sf::Mouse::Button::Left) {
                        mouseClicked = true; // <-- true solo nel frame del click
                    }
                }
                if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
                    sf::Vector2i mousePixelPos = sf::Mouse::getPosition(window);
                    mouseWorldPos = window.mapPixelToCoords(mousePixelPos);
                }
                if(scene==1|scene==2){
                    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                        if (keyPressed->code == sf::Keyboard::Key::Left&&rewinder>0) {
                            rewinder--;
                        }
                    }
                    if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                        if (keyPressed->code == sf::Keyboard::Key::Right&&rewinder<turn) {
                            rewinder++;
                    }
                }
                if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()){
                        if (keyPressed->code == sf::Keyboard::Key::Space) {
                            for(int i=0;i<pPezzi.size();i++){
                                cout<<pPezzi[i].pos<<" "<<pPezzi[i].valore<<endl;
                            }
                            cout<<calcolo_punteggio()<<endl;
                        }
                    }

            }
        }
        window.clear(sf::Color(0,0,0));
        if(scene==0){
            if(LocalB.bounds.contains(mouseWorldPos)){
                LocalB.sprite.setScale({0.85f,0.85f});
                if(mouseClicked){
                    color=0;
                    reset();
                    setSprites();
                    scene=1;
                }
            }
            else {LocalB.sprite.setScale({0.8f,0.8f});}
            if(OnlineB.bounds.contains(mouseWorldPos)){
                OnlineB.sprite.setScale({0.85f,0.85f});
                if(mouseClicked){
                    std::cout << "Connessione in corso a " << serverIp << "..." << std::endl;
                    sf::Socket::Status status = socket.connect(serverIp, port);
                    if (status != sf::Socket::Status::Done) {
                        std::cerr << "Errore: Impossibile connettersi al server!" << std::endl;
                        scene=0;
                    }else{
                        std::cout << "Connesso con successo!" << std::endl;
                        cout<<"waiting for opponent"<<endl;
                        socket.setBlocking(false);
                        scene=6;
                    }
                    
                }
            }else {OnlineB.sprite.setScale({0.8f,0.8f});}
            window.draw(home);
            window.draw(LocalB.sprite);
            window.draw(OnlineB.sprite);
            
        }
        else if(scene==6){
    sf::Socket::Status find=socket.receive(packet);
    if (find == sf::Socket::Status::Done) {
        packet>>color;
        if(color==0){Min=0;Max=7;}else{Min=-7;Max=0;}
        cout<<"avversario tovato "<<color<<endl;
        socket.setBlocking(false);
        reset();
        setSprites();
        for(int k=0;k<64;k++){
            caselle[k].sprite.setScale({0.55f,0.55f});
        }
        scene=2;
    }
    if(Cancel.bounds.contains(mouseWorldPos)){
                Cancel.sprite.setScale({0.45f,0.45f});
                if(mouseClicked){
                    scene=0;
                    socket.disconnect();
                    socket.setBlocking(true);
                }
            }
            else {Cancel.sprite.setScale({0.4f,0.4f});}
    float time = animationClock.getElapsedTime().asSeconds();
    float speed = 2.5f;       // Velocità dell'oscillazione
    float amplitude = 3.0f;  // AUMENTATO: Altezza massima del movimento (era 6.0f - troppo piccolo!)
    float offset = 1.0f;      // Sfalsamento tra i puntini

    float y1 = std::sin(time * speed) * amplitude;
    float y2 = std::sin(time * speed + offset) * amplitude;
    float y3 = std::sin(time * speed + offset * 2.0f) * amplitude;

    // CENTRA IL TESTO DENTRO LA POPUP BOX
    float textWidth = textStatic.getLocalBounds().size.x;
    float dotWidth = dot1.getLocalBounds().size.x;
    float spacing = 5.0f;
    float totalWidth = textWidth + spacing + (dotWidth * 3.0f) + (spacing * 2.0f);

    // CALCOLO CORRETTO: centra sulla popup, non sulla sua posizione top-left
    sf::FloatRect popupBounds = matchmaking.sprite.getGlobalBounds();
    float popup_center_x = popupBounds.position.x + (popupBounds.size.x / 2.0f);
    float popup_center_y = popupBounds.position.y + (popupBounds.size.y / 2.0f);
    
    // Posiziona il testo orizzontalmente centrato
    float base_x = popup_center_x - (totalWidth / 2.0f);
    float base_y = popup_center_y - 20.0f;  // Sposta un po' in alto

    // Posiziona gli elementi
    textStatic.setPosition({base_x, base_y});
    float y_offset = -10.0f;  // Sposta i pallini 5px in alto

    dot1.setPosition({base_x + textWidth + spacing, base_y + y_offset + y1});
    dot2.setPosition({base_x + textWidth + spacing + dotWidth + spacing, base_y + y_offset + y2});
    dot3.setPosition({base_x + textWidth + spacing + (dotWidth * 2.0f) + (spacing * 2.0f), base_y + y_offset + y3});

    // DISEGNO
    window.draw(home);
    window.draw(LocalB.sprite);
    window.draw(OnlineB.sprite);
    
    window.draw(matchmaking.sprite);  // Popup box
    
    // Testi animati
    window.draw(textStatic);
    window.draw(dot1);
    window.draw(dot2);
    window.draw(dot3);
    window.draw(Cancel.sprite);
}
        else if(scene==1){
            bool reset=false;
            for(int k=0;k<64;k++){
                if(caselle[k].boundspedina.contains(mouseWorldPos)&&StartPos!=k){
                    caselle[k].sprite.setScale({0.6f,0.6f});
                    int ok;
                    if(caselle[k].pedina<0){ok =1;}else{ok=0;}
                    if(mouseClicked&&rewinder==turn&&caselle[k].pedina!=0&&turn%2==ok){
                        StartPos=k;
                        cout<<"c"<<endl;
                        caselle[StartPos].PossibleMoves();
                    }else if(mouseClicked&&rewinder==turn&&StartPos!=-1&&ranges::contains(caselle[StartPos].moves,k)){
                        EndPos=k;
                        if((EndPos<8&&EndPos>=0&&caselle[StartPos].pedina==1)|(EndPos<64&&EndPos>=56&&caselle[StartPos].pedina==-1)){
                            game=1;
                            scene=67;
                            int cols=1;
                            if(caselle[StartPos].pedina<0){cols=-1;}
                            if(cols==-1){
                                prom[0].setTexture(TreginaB);
                                prom[1].setTexture(TtorreB);
                                prom[2].setTexture(TalfieraB);
                                prom[3].setTexture(TcavalloB);
                            }else{
                                prom[0].setTexture(TreginaR);
                                prom[1].setTexture(TtorreR);
                                prom[2].setTexture(TalfieraR);
                                prom[3].setTexture(TcavalloR);
                            }
                        }else{
                            mossa();
                        }
                    }else if(mouseClicked){reset=true;}
                }else if(StartPos!=k){caselle[k].sprite.setScale({0.55f,0.55f});}
            }
            if(reset){
                StartPos=-1;
                EndPos=-1;
            }
            if(Impostazioni.bounds.contains(mouseWorldPos)){
                Impostazioni.sprite.setScale({0.52f,0.52f});
                if(mouseClicked){
                    scene=0;
                }
            }else{Impostazioni.sprite.setScale({0.50f,0.50f});}
            window.draw(Impostazioni.sprite);
            cerchiolini();
            drawScacchiera();
        }else if(scene==2){
            
            sf::Socket::Status status=socket.receive(packet);
            if(status==sf::Socket::Status::Done){
                int at;
                packet>>at;
                if(at==1){
                    vector <signed char> pos;
                    packet>>pos;
                    cout<<"recv"<<endl;
                    turn++;
                    rewinder=turn;
                    cronologia.push_back(pos);
                }else if(at==2){
                    scene=0;
                    socket.disconnect();
                    socket.setBlocking(true);
                }
            }
            bool reset=false;
            for(int k=0;k<64;k++){
                if(caselle[k].boundspedina.contains(mouseWorldPos)&&StartPos!=k){
                    caselle[k].sprite.setScale({0.6f,0.6f});
                    if(turn%2==color){
                        if(mouseClicked&&rewinder==turn&&caselle[k].pedina!=0&&caselle[k].pedina>Min&&caselle[k].pedina<Max){
                            StartPos=k;
                            caselle[StartPos].PossibleMoves();
                        }else if(mouseClicked&&StartPos!=-1&&ranges::contains(caselle[StartPos].moves,k)){
                            EndPos=k;
                            if((EndPos<8&&EndPos>=0&&caselle[StartPos].pedina==1)|(EndPos<64&&EndPos>=56&&caselle[StartPos].pedina==-1)){
                                scene=67;
                                int cols=1;
                                game=2;
                                if(caselle[StartPos].pedina<0){cols=-1;}
                                if(cols==-1){
                                    prom[0].setTexture(TreginaB);
                                    prom[1].setTexture(TtorreB);
                                    prom[2].setTexture(TalfieraB);
                                    prom[3].setTexture(TcavalloB);
                                }else{
                                    prom[0].setTexture(TreginaR);
                                    prom[1].setTexture(TtorreR);
                                    prom[2].setTexture(TalfieraR);
                                    prom[3].setTexture(TcavalloR);
                                }
                            }else{
                                mossa();
                                packet.clear();
                                packet<<cronologia.back();
                                socket.send(packet);
                                packet.clear();
                            }
                        }else if(mouseClicked){reset=true;}
                    }
                }else if(StartPos!=k){caselle[k].sprite.setScale({0.55f,0.55f});}
            }
            if(reset){
                StartPos=-1;
                EndPos=-1;
            }
            if(Impostazioni.bounds.contains(mouseWorldPos)){
                Impostazioni.sprite.setScale({0.52f,0.52f});
                if(mouseClicked){
                    scene=0;
                    socket.disconnect();
                    socket.setBlocking(true);
                }
            }else{Impostazioni.sprite.setScale({0.50f,0.50f});}
            window.draw(Impostazioni.sprite);
            cerchiolini();
            drawScacchiera();
        }
        else if(scene==67){
            int cols=1;
            if(caselle[StartPos].pedina<0){cols=-1;}
            for(int i=0;i<4;i++){
                if(boundsProms[i].contains(mouseWorldPos)){
                    prom[i].setScale({0.6f,0.6f});
            if (mouseClicked) {
                switch (i){
                    case 0:
                    caselle[EndPos].pedina=6*cols;
                    break;
                    case 1:
                    caselle[EndPos].pedina=2*cols;
                    break;
                    case 2:
                    caselle[EndPos].pedina=3*cols;
                    break;
                    case 3:
                    caselle[EndPos].pedina=4*cols;
                    break;
                }
                caselle[StartPos].pedina=0;
                turn++;
                if(caselle[EndPos].pedina!=0){
                    auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==EndPos;});
                    if(cit!=pPezzi.end()){
                        cit->pos=-1;
                        pezzi_mangiati();
                    }
                }
                auto it=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==StartPos;});
                if(it!=pPezzi.end()){
                    it->pos=EndPos;
                    switch (caselle[EndPos].pedina)
                    {
                    case 6:
                        it->valore=8;
                        break;
                    
                    case -6:
                        it->valore=-8;
                        break;
                    case 2:
                        it->valore=5;
                        break;
                    
                    case -2:
                        it->valore=-5;
                        break;
                    case 3:
                    case 4:
                        it->valore=3;
                        break;
                    
                    case -3:
                    case -4:
                        it->valore=-3;
                        break;
                    }
                }
                    rewinder=turn;
                    vector <signed char>turns;
                    for(int i=0;i<64;i++){
                        turns.push_back(caselle[i].pedina);
                    }
                    cronologia.push_back(turns);
                    if(CheckForChecks(StartPos,-2)==-1){
                        if(CheckForCheckmate(StartPos)==-1){cout<<"checkmate"<<endl;scene=0;}//idk what appen fix it
                    }else{if(CheckForCheckmate(StartPos)==-1){cout<<"stalemate"<<endl;scene=0;}}
                    EndPos=-1;
                    StartPos=-1;
                    if(game==2){
                        packet.clear();
                        packet<<cronologia.back();
                        socket.send(packet);
                        packet.clear();
                        scene=2;
                    }else{
                        scene=1;
                    }
                }
            }else{prom[i].setScale({0.55f,0.55f});}
        }
        drawScacchiera();
        for(int j=0;j<4;j++){
                window.draw(prom[j]);
            }
        }
        window.display();
    }
}