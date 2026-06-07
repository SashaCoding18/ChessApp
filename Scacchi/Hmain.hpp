#include<SFML/Graphics.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <vector>
#include <cstdint>
#include <algorithm>
using namespace std;

int StartPos=-1, EndPos=-1, rewinder=0, turn=0,scene=0,color=0,Min=0,Max=0,game=0; 
class casella;
class pezzo;
sf::Font font("assets/font.otf");
sf::Vector2f mouseWorldPos;
sf::Texture Homepage("Images/Homepage.png"), Lbutton("Images/Loca game.png"),Obutton("Images/Online.png");
sf::Texture Null160("Images/Null160.png"),Tcircle("Images/Possible.png"), textureScacchiera("Images/Scacchiera2.png");
sf::Texture TpedoneB("Images/PedoneBlu.png"), TtorreB("Images/TorreBlu.png"),TalfieraB("Images/AlfiereBlu.png"),TreB("Images/ReBlu.png"),TreginaB("Images/ReginaBlu.png"),TcavalloB("Images/CavalloBlu.png");
sf::Texture TpedoneR("Images/PedoneRosso.png"),TtorreR("Images/TorreRossa.png"),TalfieraR("Images/AlfiereRosso.png"),TcavalloR("Images/CavalloRosso.png"),TreR("Images/ReRosso.png"),TreginaR("Images/ReginaRossa.png");
sf::Texture Null("Images/Null.png"), searching("Images/search.png"),rotella("Images/impostazioniBG.png"),BG("Images/XXX.png"),x("Images/cancel.png"),ex("Images/exit.png");
sf::Sprite scacchiera(textureScacchiera),home(Homepage),searchings(searching);
vector <sf::Sprite> prom,Moving,pezziBmang,pezziRmang;
vector <casella> caselle;
vector <vector<signed char>> cronologia;
sf::FloatRect boundsPedone;
vector <sf::FloatRect> boundsProms;
sf::FloatRect boundsScacchiera= scacchiera.getGlobalBounds();
sf::RenderWindow window(sf::VideoMode({1280, 720}), "Chess");
sf::View view(sf::FloatRect({0.f, 0.f}, {1280.f, 720.f}));
sf::Text player1(font),player2(font),plus1(font),plus2(font);
int CheckForChecks(int Pos1,int Pos2);
void pezzi_mangiati();
vector <pezzo> pPezzi;


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
class pezzo{
    public:
    int pos;
    int valore;
    pezzo(int x, int v){
        pos=x;
        valore=v;
    }
};
class Button{
    public:
    sf::FloatRect bounds;
    sf::Sprite sprite{Lbutton};
    Button(const sf::Texture& texture, float DimX, float DimY, float PosX, float PosY){
        sprite.setTexture(texture);
        sprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(sprite.getTexture().getSize())));
        bounds = sprite.getLocalBounds();
        sprite.setScale({DimX , DimY});
        sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
        sprite.setPosition({ PosX, PosY });
        bounds = sprite.getGlobalBounds();
    }
};

class casella{
    public:
    sf::FloatRect boundspedina;
    sf::Sprite sprite{Null160};
    int pedina,linea,colonna,n;
    bool FirstMove=true;
    vector <int> moves;
    double x,y;
    casella(int g,int PedinaCasella, double px,double py){
        n=g;
        pedina=PedinaCasella;
        x=px;
        y=py;
        linea=(g/8)+1;
        colonna=g%8+1;
    }
    void settingTexture(){
        switch(pedina){
            case -1:
            sprite.setTexture(TpedoneB);
            break;
            case -2:
            sprite.setTexture(TtorreB);
            break;
            case -3:
            sprite.setTexture(TalfieraB);
            break;
            case -4:
            sprite.setTexture(TcavalloB);
            break;
            case -5:
            sprite.setTexture(TreB);
            break;
            case -6:
            sprite.setTexture(TreginaB);
            break;
            case 1:
            sprite.setTexture(TpedoneR);
            break;
            case 2:
            sprite.setTexture(TtorreR);
            break;
            case 3:
            sprite.setTexture(TalfieraR);
            break;
            case 4:
            sprite.setTexture(TcavalloR);
            break;
            case 5:
            sprite.setTexture(TreR);
            break;
            case 6:
            sprite.setTexture(TreginaR);
            break;
            default:
            sprite.setTexture(Null160);
            break; 
        }; 
            sprite.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(sprite.getTexture().getSize())));
            sf::FloatRect bounds = sprite.getLocalBounds();
            sprite.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
            sprite.setPosition({ (float)x, (float)y });
            //sprite.setScale({0.55f,0.55f});
            boundspedina = sprite.getGlobalBounds();
    }

    void PossibleMoves(){
            moves.clear();
            switch (pedina){
            case 1:
                if(linea==4&&colonna!=1&& caselle[n-1].pedina==-1&&cronologia[turn-1][n-17]==-1){moves.push_back(n-9);}
                if(linea==4&&colonna!=8&& caselle[n+1].pedina==-1&&cronologia[turn-1][n-15]==-1){moves.push_back(n-7);}
                if(FirstMove==true&&n-16>=0){
                    if(caselle[n-8].pedina==0&&caselle[n-16].pedina==0&&CheckForChecks(n,n-16)==0){
                        moves.push_back(n-16);
                    }
                }
                if(n-8>=0){
                    if(caselle[n-8].pedina==0&&CheckForChecks(n,n-8)==0){
                        moves.push_back(n-8);
                    }
                }
                if(n-9>=0&&caselle[n-9].pedina*-1>0&&caselle[n].colonna>caselle[n-9].colonna&&CheckForChecks(n,n-9)==0){
                    moves.push_back(n-9);
                }
                if(n-7>=0&&caselle[n-7].pedina*-1>0&&caselle[n].colonna<caselle[n-7].colonna&&CheckForChecks(n,n-7)==0){
                    moves.push_back(n-7);
                }
                break;
            case -1:
                if(linea==5&&colonna!=1&& caselle[n-1].pedina==1&&cronologia[turn-1][n+15]==1){moves.push_back(n+7);}
                if(linea==5&&colonna!=8&& caselle[n+1].pedina==1&&cronologia[turn-1][n+17]==1){moves.push_back(n+9);}
                if(FirstMove==true&&n+16<64){
                    if(caselle[n+8].pedina==0&&caselle[n+16].pedina==0&&CheckForChecks(n,n+16)==0){
                        moves.push_back(n+16);
                    }
                }
                if(n+8<64){
                    if(caselle[n+8].pedina==0&&CheckForChecks(n,n+8)==0){
                        moves.push_back(n+8);
                    }
                }
                if(n+9<64&&caselle[n].colonna<caselle[n+9].colonna&&caselle[n+9].pedina*-1<0&&CheckForChecks(n,n+9)==0){
                    moves.push_back(n+9);
                }
                if(n+7<64&&caselle[n+7].pedina*-1<0&&caselle[n].colonna>caselle[n+7].colonna&&CheckForChecks(n,n+7)==0){
                    moves.push_back(n+7);
                }
                break;
            case 6:
            case -6:
            case 2:
            case -2:
                for(int i=1;i<8;i++){
                    if(n+i*8<64&&caselle[n+i*8].pedina*pedina<=0&&CheckForChecks(n,n+i*8)==0){
                        moves.push_back(n+i*8);  
                    }
                    if(n+i*8<64&&caselle[n+i*8].pedina*pedina!=0){
                            break;}
                }
                for(int i=1;i<8;i++){
                    if(n-i*8>=0&&caselle[n-i*8].pedina*pedina<=0&&CheckForChecks(n,n-i*8)==0){
                        moves.push_back(n-i*8);
                    }                        if(n-i*8>=0&&caselle[n-i*8].pedina*pedina!=0){
                            break;}
                }
                for(int i=1;i<8;i++){
                    if(n+i<64&&caselle[n+i].pedina*pedina<=0&&caselle[n+i].linea==linea&&CheckForChecks(n,n+i)==0){
                        moves.push_back(n+i);
                    }
                    if(n+i<64&&caselle[n+i].pedina*pedina!=0){
                            break;}
                }
                for(int i=1;i<8;i++){
                    if(n-i>=0&&caselle[n-i].pedina*pedina<=0&&caselle[n-i].linea==linea&&CheckForChecks(n,n-i)==0){
                        moves.push_back(n-i);
                    }
                    if(n-i>=0&&caselle[n-i].pedina*pedina!=0){
                            break;}
                }
                if(pedina!=6&&pedina!=-6){break;}
            case 3:
            case -3:
                for(int i=1;i<8;i++){
                    if(n-i*9>=0&&caselle[n-i*9].pedina*pedina<=0&&caselle[n-i*9].colonna<colonna&&CheckForChecks(n,n-i*9)==0){
                        moves.push_back(n-i*9);
                    }
                    if(n-i*9>=0&&caselle[n-i*9].pedina*pedina!=0){break;}
                }
                for(int i=1;i<8;i++){
                    if(n+i*9<64&&caselle[n+i*9].pedina*pedina<=0&&caselle[n+i*9].colonna>colonna&&CheckForChecks(n,n+i*9)==0){
                        moves.push_back(n+i*9);
                    }if(n+i*9<64&&caselle[n+i*9].pedina*pedina!=0){break;}
                }
                for(int i=1;i<8;i++){
                    if(n-i*7>=0&&caselle[n-i*7].pedina*pedina<=0&&caselle[n-i*7].colonna>colonna&&CheckForChecks(n,n-i*7)==0){
                        moves.push_back(n-i*7);
                    }if(n-i*7>=0&&caselle[n-i*7].pedina*pedina!=0){break;}
                }
                for(int i=1;i<8;i++){
                    if(n+i*7<64&&caselle[n+i*7].pedina*pedina<=0&&caselle[n+i*7].colonna<colonna&&CheckForChecks(n,n+i*7)==0){
                        moves.push_back(n+i*7);
                    }if(n+i*7<64&&caselle[n+i*7].pedina*pedina!=0){break;}
                }
                break;
            case 5:
            case -5:
                for(int i=-1;i<2;i++){
                    if(n+i-8>=0&&caselle[n+i-8].pedina*pedina<=0&&linea-1==caselle[n+i-8].linea&&CheckForChecks(n,n+i-8)==0){
                        moves.push_back(n+i-8);
                    }
                }
                for(int i=-1;i<2;i++){
                    if(n+i+8<64&&caselle[n+i+8].pedina*pedina<=0&&linea+1==caselle[n+i+8].linea&&CheckForChecks(n,n+i+8)==0){
                        moves.push_back(n+i+8);
                    }
                }
                if(n+1<64&&caselle[n+1].pedina*pedina<=0&&linea==caselle[n+1].linea&&CheckForChecks(n,n+1)==0){
                        moves.push_back(n+1);}
                if(n-1>=0&&caselle[n-1].pedina*pedina<=0&&linea==caselle[n-1].linea&&CheckForChecks(n,n-1)==0){
                        moves.push_back(n-1);}
                if(pedina==5&&FirstMove&&caselle[63].FirstMove&&caselle[62].pedina==0&&caselle[61].pedina==0){moves.push_back(n+2);}
                if(pedina==5&&FirstMove&&caselle[56].FirstMove&&caselle[57].pedina==0&&caselle[58].pedina==0&&caselle[59].pedina==0){moves.push_back(n-2);}
                if(pedina==-5&&FirstMove&&caselle[7].FirstMove&&caselle[6].pedina==0&&caselle[5].pedina==0){moves.push_back(n+2);}
                if(pedina==-5&&FirstMove&&caselle[0].FirstMove&&caselle[1].pedina==0&&caselle[2].pedina==0&&caselle[3].pedina==0){moves.push_back(n-2);}
                break;
                case 4:
                case -4:
                if(n+10<64&&caselle[n+10].pedina*pedina<=0&&colonna<caselle[n+10].colonna&&CheckForChecks(n,n+10)==0){moves.push_back(n+10);}
                if(n-10>=0&&caselle[n-10].pedina*pedina<=0&&colonna>caselle[n-10].colonna&&CheckForChecks(n,n-10)==0){moves.push_back(n-10);}
                if(n+17<64&&caselle[n+17].pedina*pedina<=0&&colonna<caselle[n+17].colonna&&CheckForChecks(n,n+17)==0){moves.push_back(n+17);}
                if(n-17>=0&&caselle[n-17].pedina*pedina<=0&&colonna>caselle[n-17].colonna&&CheckForChecks(n,n-17)==0){moves.push_back(n-17);}
                if(n+6<64&&caselle[n+6].pedina*pedina<=0&&colonna>caselle[n+6].colonna&&CheckForChecks(n,n+6)==0){moves.push_back(n+6);}
                if(n-6>=0&&caselle[n-6].pedina*pedina<=0&&colonna<caselle[n-6].colonna&&CheckForChecks(n,n-6)==0){moves.push_back(n-6);}
                if(n+15<64&&caselle[n+15].pedina*pedina<=0&&colonna>caselle[n+15].colonna&&CheckForChecks(n,n+15)==0){moves.push_back(n+15);}
                if(n-15>=0&&caselle[n-15].pedina*pedina<=0&&colonna<caselle[n-15].colonna&&CheckForChecks(n,n-15)==0){moves.push_back(n-15);}
        };
    }
};

int CheckForChecks(int Pos1,int Pos2){
        int temp,temp1,color;
        if(caselle[Pos1].pedina*-1>=0){color=-1;}else{color=1;}
        if(Pos2>0){
            if(caselle[Pos1].pedina==-5){
                pPezzi[15].pos=Pos2;
            }
            if(caselle[Pos1].pedina==5){
                pPezzi[31].pos=Pos2;
            }
            temp=caselle[Pos1].pedina;
            temp1=caselle[Pos2].pedina;
            caselle[Pos2].pedina=caselle[Pos1].pedina;
            caselle[Pos1].pedina=0;
        }else{color=color*-1;}
        int king=-1;
        if(color==-1){king=pPezzi[15].pos;cout<<pPezzi[15].pos<<endl;}else{king=pPezzi[31].pos;cout<<pPezzi[31].pos<<endl;}
        auto safe_exit =[&](int result){
            if (Pos2 > 0) {
                caselle[Pos1].pedina = temp;
                caselle[Pos2].pedina = temp1;
                if(caselle[Pos1].pedina==-5){
                    pPezzi[15].pos=Pos1;
                }
                if(caselle[Pos1].pedina==5){
                    pPezzi[31].pos=Pos1;
                }
            }
        return result;
        };
        for(int i=1;i<9;i++){
            if(king-(i*8)>=0){
            if(caselle[king-(i*8)].pedina==-2*color|caselle[king-(i*8)].pedina==-6*color){
                return safe_exit(-1);
            }else if(caselle[king-(i*8)].pedina!=0){i=101;}
        }else{i=67;}}
        for(int i=1;i<9;i++){
            if(king-(i*-8)<64){
            if(caselle[king-(i*-8)].pedina==-2*color|caselle[king-(i*-8)].pedina==-6*color){
                return safe_exit(-1);
            }else if(caselle[king-(i*-8)].pedina!=0){i=101;}
        }else{i=67;}}
        for(int i=1;i<9;i++){
            if(king+i<64){
            if((caselle[king+i].pedina==-2*color|caselle[king+i].pedina==-6*color)&&caselle[king+i].linea==caselle[king].linea){
                return safe_exit(-1);
            }else if(caselle[king+i].pedina!=0&&caselle[king+i].linea==caselle[king].linea){i=101;}
        }else{i=67;}}
        for(int i=1;i<9;i++){
            if(king-i>=0){
            if((caselle[king-i].pedina==-2*color|caselle[king-i].pedina==-6*color)&&caselle[king-i].linea==caselle[king].linea){
                return safe_exit(-1);
            }else if(caselle[king-i].pedina!=0&&caselle[king-i].linea==caselle[king].linea){i=101;}
        }else{i=67;}}
        
        for(int i=1;i<9;i++){
            if(king-i*9>=0){
                if((caselle[king-i*9].pedina==-3*color|caselle[king-i*9].pedina==-6*color)&&caselle[king].colonna>caselle[king-i*9].colonna){
                    return safe_exit(-1);
                }else if(caselle[king-i*9].pedina!=0){i=104;}
            }else{i=67;}
        }
        for(int i=1;i<9;i++){
            if(king-i*7>=0){
                if((caselle[king-i*7].pedina==-3*color|caselle[king-i*7].pedina==-6*color)&&caselle[king].colonna<caselle[king-i*7].colonna){
                    return safe_exit(-1);
                }else if(caselle[king-i*7].pedina!=0){i=104;}
            }else{i=67;}
        }
        for(int i=1;i<9;i++){
            if(king+i*9<64){
                if((caselle[king+i*9].pedina==-3*color|caselle[king+i*9].pedina==-6*color)&&caselle[king].colonna<caselle[king+i*9].colonna){
                    return safe_exit(-1);
                }else if(caselle[king+i*9].pedina!=0){i=104;}
            }else{i=67;}
        }
        for(int i=1;i<9;i++){
            if(king+i*7<64){
                if((caselle[king+i*7].pedina==-3*color|caselle[king+i*7].pedina==-6*color)&&caselle[king].colonna>caselle[king+i*7].colonna){
                    return safe_exit(-1);
                }else if(caselle[king+i*7].pedina!=0){i=104;}
            }else{i=67;}
        }
        if(king-9*color>=0&&king-9*color<64){
        if(caselle[king-9*color].pedina==-1*color){return safe_exit(-1);}}
        if(king-7*color>=0&&king-7*color<64){
        if(caselle[king-7*color].pedina==-1*color){return safe_exit(-1);}}
        
        if(king-10>=0&&king-10<64){
            if(caselle[king-10].pedina==-4*color&&caselle[king].colonna>caselle[king-10].colonna){
                return safe_exit(-1);
            }
        }
        if(king+10<64&&king+10<64){
            if(caselle[king+10].pedina==-4*color&&caselle[king].colonna<caselle[king+10].colonna){
                return safe_exit(-1);
            }
        }
        if(king-17>=0&&king-17<64){
            if(caselle[king-17].pedina==-4*color&&caselle[king].colonna>caselle[king-17].colonna){
                return safe_exit(-1);
            }
        }
        if(king+17<64&&king+17<64){
            if(caselle[king+17].pedina==-4*color&&caselle[king].colonna<caselle[king+17].colonna){
                return safe_exit(-1);
            }
        }
        if(king-6>=0&&king-6<64){
            if(caselle[king-6].pedina==-4*color&&caselle[king].colonna<caselle[king-6].colonna){
                return safe_exit(-1);
            }
        }
        if(king-15>=0&&king-15<64){
            if(caselle[king-15].pedina==-4*color&&caselle[king].colonna<caselle[king-15].colonna){
                return safe_exit(-1);
            }
        }
        if(king+6>=0&&king+6<64){
            if(caselle[king+6].pedina==-4*color&&caselle[king].colonna>caselle[king+6].colonna){
                return safe_exit(-1);
            }
        }
        if(king+15>=0&&king+15<64){
            if(caselle[king+15].pedina==-4*color&&caselle[king].colonna>caselle[king+15].colonna){
                return safe_exit(-1);
            }
        }
        for(int i=-1;i<2;i++){
            if(king-8+i>=0){
                if(caselle[king-8+i].pedina==-5*color&&caselle[king].linea-1==caselle[king-8+i].linea){
                    return safe_exit(-1);
                }
            }
        }
        for(int i=-1;i<2;i++){
            if(king+8+i<64){
                if(caselle[king+8+i].pedina==-5*color&&caselle[king].linea+1==caselle[king+8+i].linea){
                    return safe_exit(-1);
                }
            }
        }
        if(king+1<64){
            if(caselle[king+1].pedina==-5*color&&caselle[king].linea==caselle[king+1].linea){
                return safe_exit(-1);
            }
        }
        if(king-1>=0){
            if(caselle[king-1].pedina==-5*color&&caselle[king].linea==caselle[king-1].linea){
                return safe_exit(-1);
            }
        }
        return safe_exit(0);

    }

int CheckForCheckmate(int x){
        int col;
        if(x*-1>=0){col=-1;}else{col=1;}
        cout<<col<<endl;
        for(int i=0;i<64;i++){
            if(caselle[i].pedina*col>0){
            caselle[i].PossibleMoves();
            if(caselle[i].moves.size()>0){
                return 0;
            }}}
        return -1;
    }

void mossa(){
    if(caselle[StartPos].linea==4&&EndPos==StartPos-9){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[StartPos-1].pedina=0;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==StartPos-1;});
        if(cit!=pPezzi.end()){
            cit->pos=-1;
        }
    }else if(caselle[StartPos].linea==4&&EndPos==StartPos-7){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[StartPos+1].pedina=0;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==StartPos+1;});
        if(cit!=pPezzi.end()){
            cit->pos=-1;
        }
    }else if(caselle[StartPos].linea==5&&EndPos==StartPos+7){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[StartPos-1].pedina=0;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==StartPos-1;});
        if(cit!=pPezzi.end()){
            cit->pos=-1;
        }
    }else if(caselle[StartPos].linea==5&&EndPos==StartPos+9){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[StartPos+1].pedina=0;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==StartPos+1;});
        if(cit!=pPezzi.end()){
            cit->pos=-1;
        }
    }else if(StartPos==60&&EndPos==62){
        caselle[EndPos].pedina=caselle[60].pedina;
        caselle[60].pedina=0;
        caselle[61].pedina=caselle[63].pedina;
        caselle[63].pedina=0;
        caselle[63].FirstMove=false;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==63;});
        if(cit!=pPezzi.end()){
            cit->pos=61;
        }
    }else if(StartPos==60&&EndPos==58){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[59].pedina=caselle[56].pedina;
        caselle[56].pedina=0;
        caselle[56].FirstMove=false;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==56;});
        if(cit!=pPezzi.end()){
            cit->pos=59;
        }
    }else if(StartPos==4&&EndPos==6){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[5].pedina=caselle[7].pedina;
        caselle[7].pedina=0;
        caselle[7].FirstMove=false;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==7;});
        if(cit!=pPezzi.end()){
            cit->pos=5;
        }
    }else if(StartPos==4&&EndPos==2){
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[3].pedina=caselle[0].pedina;
        caselle[0].pedina=0;
        caselle[0].FirstMove=false;
        auto cit=std::find_if(pPezzi.begin(),pPezzi.end(),[](const pezzo& p){return p.pos==0;});
        if(cit!=pPezzi.end()){
            cit->pos=3;
        }
    }
    else{
        caselle[EndPos].pedina=caselle[StartPos].pedina;
        caselle[StartPos].pedina=0;
        caselle[EndPos].FirstMove=false;
        caselle[StartPos].FirstMove=false;
    }
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
    }
    if(CheckForChecks(StartPos,-2)==-1){
        if(CheckForCheckmate(StartPos)==-1){cout<<"checkmate"<<endl;scene=0;}
    }else{if(CheckForCheckmate(StartPos)==-1){cout<<"stalemate"<<endl;scene=0;}}
    StartPos=-1;
    EndPos=-1;
    rewinder=turn;
    vector <signed char>turno;
    for(int i=0;i<64;i++){
        turno.push_back(caselle[i].pedina);
    }
    cronologia.push_back(turno);
}

void setSprites(){
    //text
    player1.setString("Player 1");
    player1.setCharacterSize(20);
    player1.setFillColor(sf::Color::White);
    player1.setPosition({20.f,20.f});
    player2.setString("Player 2");
    player2.setCharacterSize(20);
    player2.setFillColor(sf::Color::White);
    player2.setPosition({20.f,590.f});
    pPezzi={{8,-1},{9,-1},{10,-1},{11,-1},{12,-1},{13,-1},{14,-1},{15,-1},{1,-3},{6,-3},{2,-3},{5,-3},{0,-5},{7,-5},{3,-8},{4,-10000},{48,1},{49,1},{50,1},{51,1},{52,1},{53,1},{54,1},{55,1},{57,3},{62,3},{58,3},{61,3},{56,5},{63,5},{59,8},{60,10000}};//(torreB,torreB,cavalloB,cavalloB,alfiereB,alfiereB,ReginaB,ReB,PedoneBx8,same for red)
    //scacchiera
    boundsScacchiera= scacchiera.getLocalBounds();
    scacchiera.setOrigin(boundsScacchiera.size.componentWiseMul({0.5f,0.5f}));
    scacchiera.setPosition({640.f,360.f});
    scacchiera.setScale({0.7f,0.7f});
    boundsScacchiera= scacchiera.getLocalBounds();
    //sprite x promozione
    for(int i=0;i<4;i++){
    sf::Sprite x(Null160);
    prom.push_back(x);
    prom[i].setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(prom[i].getTexture().getSize())));
    sf::FloatRect temp = prom[i].getLocalBounds();
    boundsProms.push_back(temp);
    prom[i].setOrigin({ boundsProms[i].size.x / 2.f, boundsProms[i].size.y / 2.f });
    prom[i].setScale({0.55f,0.55f});
    prom[i].setPosition({ 230.f, (float)(225+(90*i)) });
    boundsProms[i] = prom[i].getGlobalBounds();
}   
    //pedine
    if(color==0){
        casella test(0,-2,320,45);
        caselle.push_back(test);
        casella test2(1,-4,410,45);
        caselle.push_back(test2);
        casella test3(2,-3,500,45);
        caselle.push_back(test3);
        casella test4(3,-6,590,45);
        caselle.push_back(test4);
        casella test5(4,-5,680,45);
        caselle.push_back(test5);
        casella test6(5,-3,770,45);
        caselle.push_back(test6);
        casella test7(6,-4,860,45);
        caselle.push_back(test7);
        casella test8(7,-2,950,45);
        caselle.push_back(test8);
    for(int i=0;i<8;i++){
        casella bossetti(i+8,-1,320+(i*90),135);
        caselle.push_back(bossetti);
    }
    for(int i=0;i<4;i++){
        for(int j=0;j<8;j++){
        casella trump((i*8)+j+16,0,320+(j*90),225+(i*90));
        caselle.push_back(trump);
        }
    }
    for(int i=0;i<8;i++){
        casella epstein(i+48,1,320+(i*90),585);
        caselle.push_back(epstein);
    }
    casella test1(56,2,320,675);
    caselle.push_back(test1);
    casella test12(57,4,410,675);
    caselle.push_back(test12);
    casella test13(58,3,500,675);
    caselle.push_back(test13);
    casella test14(59,6,590,675);
    caselle.push_back(test14);
    casella test15(60,5,680,675);
    caselle.push_back(test15);
    casella test16(61,3,770,675);
    caselle.push_back(test16);
    casella test17(62,4,860,675);
    caselle.push_back(test17);
    casella test18(63,2,950,675);
    caselle.push_back(test18);
    }else{
        casella test(0,-2,950,675);
        caselle.push_back(test);
        casella test2(1,-4,860,675);
        caselle.push_back(test2);
        casella test3(2,-3,770,675);
        caselle.push_back(test3);
        casella test4(3,-6,680,675);
        caselle.push_back(test4);
        casella test5(4,-5,590,675);
        caselle.push_back(test5);
        casella test6(5,-3,500,675);
        caselle.push_back(test6);
        casella test7(6,-4,410,675);
        caselle.push_back(test7);
        casella test8(7,-2,320,675);
        caselle.push_back(test8);
        for(int i=0;i<8;i++){
            casella bossetti(i+8,-1,950-(i*90),585);
            caselle.push_back(bossetti);
        }
        for(int i=0;i<4;i++){
            for(int j=0;j<8;j++){
                casella trump((i*8)+j+16,0,950-(j*90),495-(i*90));
                caselle.push_back(trump);
            }
        }
        for(int i=0;i<8;i++){
            casella epstein(i+48,1,950-(i*90),135);
            caselle.push_back(epstein);
        }
        casella test1(56,2,950,45);
        caselle.push_back(test1);
        casella test12(57,4,860,45);
        caselle.push_back(test12);
        casella test13(58,3,770,45);
        caselle.push_back(test13);
        casella test14(59,6,680,45);
        caselle.push_back(test14);
        casella test15(60,5,590,45);
        caselle.push_back(test15);
        casella test16(61,3,500,45);
        caselle.push_back(test16);
        casella test17(62,4,410,45);
        caselle.push_back(test17);
        casella test18(63,2,320,45);
        caselle.push_back(test18);
    }
    
    //segna possibili mosse
    for(int i=0;i<64;i++){
        sf::Sprite gigi(Null);
        Moving.push_back(gigi);
        sf::FloatRect boundsgigi = Moving[i].getGlobalBounds();
        int x,y;
        x=caselle[i].x;
        y=caselle[i].y;
        Moving[i].setOrigin({ boundsgigi.size.x / 2.f, boundsgigi.size.y / 2.f });
        Moving[i].setPosition({ (float)x, (float)y });
    }
    //inizializzazione cronologia
    vector <signed char> gianni;
    for(int i=0;i<64;i++){
        gianni.push_back(caselle[i].pedina);
    }
        cronologia.push_back(gianni);
}

void reset(){
    caselle.clear();
                cronologia.clear();
                prom.clear();
                Moving.clear();
                boundsProms.clear();
                turn=0;
                rewinder=0;
                StartPos=-1, EndPos=-1;
}
int calcolo_punteggio(){
    int tot=0;
    for(int i=0;i<pPezzi.size();i++){
        if(pPezzi[i].pos>=0){
            tot+=pPezzi[i].valore;
        }
    }
    return tot;
}

void drawScacchiera(){
    window.draw(scacchiera);
        for(int i=0;i<caselle.size();i++){
            caselle[i].pedina=cronologia[rewinder][i];
            caselle[i].settingTexture();
            window.draw(caselle[i].sprite);
            window.draw(Moving[i]);
        }
    if(pezziBmang.size()>0){
        for(int i=0;i<pezziBmang.size();i++){
            window.draw(pezziBmang[i]);
        }
    }
    if(pezziRmang.size()>0){
        for(int i=0;i<pezziRmang.size();i++){
            window.draw(pezziRmang[i]);
        }
    }
    window.draw(player1);
    window.draw(player2);
    window.draw(plus1);
    window.draw(plus2);
    //cout<<calcolo_punteggio()<<endl;
}

void cerchiolini(){
    for(int p=0;p<64;p++){
        if(StartPos!=-1&&caselle[StartPos].moves.size()>0){
            if(std::ranges::contains(caselle[StartPos].moves,p)){
                Moving[p].setTexture(Tcircle);
            }else{
                Moving[p].setTexture(Null);
            }
        }else{
            Moving[p].setTexture(Null);
        }
    }
}

void pezzi_mangiati(){
    sf::Sprite x(TpedoneR);
    x.setTextureRect(sf::IntRect({0, 0}, sf::Vector2i(x.getTexture().getSize())));
    sf::FloatRect bounds = x.getLocalBounds();
    x.setOrigin({ bounds.size.x / 2.f, bounds.size.y / 2.f });
    x.setScale({0.35f,0.35f});
    pezziBmang.clear();
    pezziRmang.clear();
    int X=20,Y=650;
    for(int i=0;i<15;i++){
        if(pPezzi[i].pos<0){
            switch (i) //(PedoneBx8,cavalloB,cavalloB,alfiereB,alfiereB,torreB,torreB,ReginaB,ReB,same for red)
            {
            case 12:
            case 13:
                x.setTexture(TtorreB);
                break;
            case 8:
            case 9:
                x.setTexture(TcavalloB);
                break;
            case 10:
            case 11:
                x.setTexture(TalfieraB);
                break;
            case 14:
                x.setTexture(TreginaB);
                break;
            default:
                x.setTexture(TpedoneB);
                break;
            }
            X+=15;
            x.setPosition({ (float)X, (float)Y });
            pezziBmang.push_back(x);
        }
    }
    int s=calcolo_punteggio();
    if(s>0){
        plus1.setString("+"+to_string(s));
        plus1.setCharacterSize(20);
        plus1.setFillColor(sf::Color::White);
        plus1.setPosition({float(X+20),float(Y-7)});
    }else{plus1.setString(" ");}
    X=20;
    Y=80;
    for(int i=16;i<31;i++){
        if(pPezzi[i].pos<0){
            switch (i) //(PedoneBx8,cavalloB,cavalloB,alfiereB,alfiereB,torreB,torreB,ReginaB,ReB,same for red)
            {
            case 28:
            case 29:
                x.setTexture(TtorreR);
                break;
            case 24:
            case 25:
                x.setTexture(TcavalloR);
                break;
            case 26:
            case 27:
                x.setTexture(TalfieraR);
                break;
            case 30:
                x.setTexture(TreginaR);
                break;
            default:
                x.setTexture(TpedoneR);
                break;
            }
            X+=15;
            x.setPosition({ (float)X, (float)Y });
            pezziRmang.push_back(x);
        }
    }
    if(s<0){
        plus2.setString("+"+to_string(s*-1));
        plus2.setCharacterSize(20);
        plus2.setFillColor(sf::Color::White);
        plus2.setPosition({float(X+20),float(Y-7)});
    }else{plus2.setString(" ");}
}