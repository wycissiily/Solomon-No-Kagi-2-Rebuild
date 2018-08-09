#include<windows.h>
#include<iostream>
#include<cstring>
#include<cstdio>
#include<queue>
using namespace std;
#define GAKS GetAsyncKeyState
const int width=30;
char ma[101][101],tmp[101],tm[101];
int fx[101][101],n,m,mx,my,to,bl,re,fire,win,bst,up,dn,S,F,M,CL,wdwd,wdhe;
bool mv[101][101],emp,ob;
HDC hDC;
void rr(){
    CONSOLE_SCREEN_BUFFER_INFO wd;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),&wd);
    wdwd=wd.srWindow.Right+1;
    wdhe=wd.srWindow.Bottom+1;
}
char leftlink(char ch){
    if(ch=='>')return 'O';
    else if(ch=='=')return '<';
    else if(ch=='L')return 'M';
    else if(ch=='D')return 'R';
    else return ch;
}
char rightlink(char ch){
    if(ch=='<')return 'O';
    else if(ch=='=')return '>';
    else if(ch=='R')return 'M';
    else if(ch=='D')return 'L';
    else return ch;
}
char oleftlink(char ch){
    if(ch=='O')return '>';
    else if(ch=='<')return '=';
    else if(ch=='M')return 'L';
    else if(ch=='R')return 'D';
    else return ch;
}
char orightlink(char ch){
    if(ch=='O')return '<';
    else if(ch=='>')return '=';
    else if(ch=='M')return 'R';
    else if(ch=='L')return 'D';
    else return ch;
}
void printco(char ch,int co,bool re){
    for(int i=1;i<=co;i++)printf("%c",ch);
    if(re)printf("\n");
}
void printmid(const char *s){
    rr();
    int len=strlen(s);
    int ko=wdwd-len,ech=ko/2,yu=ko%2;
    printco(' ',ech+yu,false);
    printf("%s",s);
    printco(' ',ech,false);
}
void printsmid(int co,const char *s[]){
    rr();
    int len=0;
    for(int i=0;i<co;i++)len+=strlen(s[i]);
    int ko=wdwd-len,ech=ko/(co+1),yu=ko%(co+1);
    for(int i=0;i<co;i++){
        if(i<yu)printf(" ");
        printco(' ',ech,false);
        printf("%s",s[i]);
    }
    printco(' ',ech,false);
}
void warning(const char *s){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
    printf("Error:%s\n",s);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
}
int choslvl(){
    system("cls");
    int lev,pl,tot;
    FILE *tmp=fopen("data\\level.txt","rb"),*save=fopen("data\\save.txt","rb");
    fread(&tot,sizeof(int),1,tmp);
    fread(&pl,sizeof(int),1,save);
    fclose(save);
    if(!tot){
        printf("无关卡！\n\n");
        system("pause");
        return -1;
    }
    lev=min(tot,pl);
    tot=min((lev%10?lev/10+1:lev/10)*10,tot);
    for(int i=1;i<lev;i++){
        int me;
        fread(&me,sizeof(int),1,tmp);
        fseek(tmp,ftell(tmp)+me,SEEK_SET);
    }
    while(true){
        system("cls");
        printmid("选择关卡");
        printf("\n\n");
        rr();
        printco('=',wdwd,true);
        printf("按esc退出\n\n");
        FILE *rd=fopen("data\\level.txt","rb");
        fseek(rd,ftell(tmp)+sizeof(int),SEEK_SET);
        fread(&n,sizeof(int),1,rd);
        fread(&m,sizeof(int),1,rd);
        for(int i=1;i<=n;i++){
            fread(&ma[i][1],sizeof(char),m,rd);
            ma[i][m+1]='\0';
        }
        fclose(rd);
        rr();
        int h=wdhe-4-n-tot/10,l,r;
        if(h>=tot){
            l=1;
            r=tot;
        }else{
            l=max(lev-h/2,1);
            r=min(lev+h/2-1,tot);
        }
        for(int i=l;i<=r;i++){
            if(lev==i)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_INTENSITY);
            if(i>pl)SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);
            printf("关卡%d-%d",(i-1)/10+1,(!(i%10)?10:i%10));
            if(lev==i){
                printf(" █\n");
                printf("    %d×%d\n",n,m);
                for(int i=1;i<=n;i++)printf("   %s\n",&ma[i][1]);
            }else printf("\n");
            if(!(i%10))printf("\n");
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
        }
        bool esc,up,down,enter;
        while((esc=GAKS(VK_ESCAPE))||(up=GAKS(VK_UP))||(down=GAKS(VK_DOWN))||(enter=GAKS(VK_RETURN)));
        while(!((esc=GAKS(VK_ESCAPE))||(up=GAKS(VK_UP))||(down=GAKS(VK_DOWN))||(enter=GAKS(VK_RETURN))));
        if(esc){
            fclose(tmp);
            return -1;
        }else if(up&&lev!=1){
            fseek(tmp,ftell(tmp)-sizeof(int),SEEK_SET);
            int me;
            fread(&me,sizeof(int),1,tmp);
            fseek(tmp,ftell(tmp)-sizeof(int)-me,SEEK_SET);
            lev--;
        }else if(down&&lev!=tot){
            int me;
            fread(&me,sizeof(int),1,tmp);
            fseek(tmp,ftell(tmp)+me,SEEK_SET);
            lev++;
        }else if(enter&&lev<=pl){
            fclose(tmp);
            bl=(lev%10?lev/10+1:lev/10);
            return lev;
        }
    }
}
void rdlvlmap(int lvl){
    int tm;
    FILE *tmp=fopen("data\\level.txt","rb");
    fread(&tm,sizeof(int),1,tmp);
    for(int i=1;i<lvl;i++){
        int me;
        fread(&me,sizeof(int),1,tmp);
        fseek(tmp,ftell(tmp)+me,SEEK_SET);
    }
    fread(&tm,sizeof(int),1,tmp);
    fread(&n,sizeof(int),1,tmp);
    fread(&m,sizeof(int),1,tmp);
    for(int i=1;i<=n;i++){
        fread(&ma[i][1],sizeof(char),m,tmp);
        ma[i][m+1]='\0';
    }
    fclose(tmp);
}
void showbmp(const char *li,int x,int y,int w=width,int h=width){
    HBITMAP hBitmap=(HBITMAP)LoadImage(NULL,li,IMAGE_BITMAP,0,0,LR_LOADFROMFILE|LR_CREATEDIBSECTION);
    HBRUSH hBrush=CreatePatternBrush(hBitmap);
    HBRUSH hOldBrush=(HBRUSH)SelectObject(hDC,hBrush);
    HPEN hPen=CreatePen(PS_NULL,0,RGB(0,0,0));
    HPEN hOldPen=(HPEN)SelectObject(hDC,hPen);
    POINT P=(POINT){0,0};
    SetViewportOrgEx(hDC,x,y-(up-1)*w,&P);
    Rectangle(hDC,0,0,w+1,h+1);
    SelectObject(hDC,hOldBrush);
    SelectObject(hDC,hOldPen);
    DeleteObject(hOldBrush);
    DeleteObject(hBrush);
    DeleteObject(hOldPen);
    DeleteObject(hPen);
    DeleteObject(hBitmap);
} 
const char* addbylvl(const char *s,int lv){
    if(lv%10!=1)sprintf(tmp,"%s%d.bmp",s,(lv<=10?lv:lv%10));
    else sprintf(tmp,"%s.bmp",s);
    return tmp;
}
const char* addbylvlnm(const char *s,int lv){
    sprintf(tmp,"%s%d.bmp",s,lv);
    return tmp;
}
const char* getlib(char c){
    if(c=='F')return addbylvl("data\\fire",bl);
    else if(c=='a')return addbylvlnm("data\\airfireud",bl);
    else if(c=='_'){
        if(emp)return "data\\empty.bmp";
        else return addbylvl("data\\back",bl);
    }else if(c=='M')return "data\\iron.bmp";
    else if(c=='L')return "data\\liceiron.bmp";
    else if(c=='R')return "data\\riceiron.bmp";
    else if(c=='D')return "data\\licericeiron.bmp";
    else if(c=='e')return "data\\empty.bmp";
    else if(c=='=')return "data\\licerice.bmp";
    else if(c=='>')return "data\\licer_.bmp";
    else if(c=='<')return "data\\l_rice.bmp";
    else if(c=='x')return "data\\firebucket_close.bmp";
    else if(c=='V')return "data\\firebucket_open.bmp";
    else if(c=='|')return "data\\UD.bmp";
    else if(c=='-')return "data\\LR.bmp";
    else if(c=='/')return "data\\UL.bmp";
    else if(c=='\\')return "data\\LD.bmp";
    else if(c=='\'')return "data\\UR.bmp";
    else if(c=='\"')return "data\\RD.bmp";
    else return "data\\l_r_.bmp";
}
void boom(int x,int y){
    showbmp(addbylvl("data\\ac_1",bl),(y-1)*width,(x+1)*width);
    Sleep(10);
    showbmp(addbylvl("data\\ac_2",bl),(y-1)*width,(x+1)*width);
    Sleep(10);
    showbmp(getlib(ma[x][y]),(y-1)*width,(x+1)*width);
}
void printmap(){
    int cl=(CL==n+1?n:CL);
    up=max(1,cl-24);
    dn=cl;
    for(int i=up;i<=min(dn+1,n);i++){
        for(int j=1;j<=m;j++){
            if(ma[i][j]=='S'){
                sprintf(tmp,"data\\boss_%d.bmp",bst);
                showbmp(tmp,(j-1)*width,(i+1)*width,180,240);
            }else if(ma[i][j]=='n'||ma[i][j]=='H')continue;
            else if(ma[i][j]=='B'){
                mx=i;
                my=j;
                if(emp){
                    if(!to)showbmp("data\\player_leftempty.bmp",(j-1)*width,(i+1)*width);
                    else showbmp("data\\player_rightempty.bmp",(j-1)*width,(i+1)*width);
                }else{
                    if(!to)showbmp(addbylvl("data\\player_left",bl),(j-1)*width,(i+1)*width);
                    else showbmp(addbylvl("data\\player_right",bl),(j-1)*width,(i+1)*width);
                }
            }else if(ma[i][j]=='f'){
                if(!fx[i][j])showbmp(addbylvlnm("data\\airfirelrl",bl),(j-1)*width,(i+1)*width);
                else showbmp(addbylvlnm("data\\airfirelrr",bl),(j-1)*width,(i+1)*width);
            }else if(ma[i][j]=='@'||ma[i][j]=='|'||ma[i][j]=='-'||ma[i][j]=='\\'||ma[i][j]=='/'||ma[i][j]=='\''||ma[i][j]=='\"'||ma[i][j]=='x'||ma[i][j]=='V'){
                if(ma[i][j]=='V'&&i==CL)showbmp("data\\superfire.bmp",(j-1)*width,(i+1)*width);
                else{
                    string tm;
                    if(ma[i][j]!='@'){
                        tm=getlib(ma[i][j]);
                        tm=tm.substr(0,tm.size()-4);
                    }
                    if(ma[i][j-1]=='<'||ma[i][j-1]=='='){
                        if(ma[i][j+1]=='='||ma[i][j+1]=='>'){
                            if(ma[i][j]!='@')sprintf(tmp,"%sLRI.bmp",tm.c_str());
                            else sprintf(tmp,"%s",addbylvl("data\\wallLRI",bl));
                            showbmp(tmp,(j-1)*width,(i+1)*width);
                        }else{
                            if(ma[i][j]!='@')sprintf(tmp,"%sLI.bmp",tm.c_str());
                            else sprintf(tmp,"%s",addbylvl("data\\wallLI",bl));
                            showbmp(tmp,(j-1)*width,(i+1)*width);
                        }
                    }else{
                        if(ma[i][j+1]=='='||ma[i][j+1]=='>'){
                            if(ma[i][j]!='@')sprintf(tmp,"%sRI.bmp",tm.c_str());
                            else sprintf(tmp,"%s",addbylvl("data\\wallRI",bl));
                            showbmp(tmp,(j-1)*width,(i+1)*width);
                        }else{
                            if(ma[i][j]!='@')sprintf(tmp,"%s.bmp",tm.c_str());
                            else sprintf(tmp,"%s",addbylvl("data\\wall",bl));
                            showbmp(tmp,(j-1)*width,(i+1)*width);
                        }
                    }
                }
            }else showbmp(getlib(ma[i][j]),(j-1)*width,(i+1)*width);
        }
    }
}
int cntfr(){
    int cl=(CL==n+1?n:CL),fir=0;
    up=max(1,cl-24);
    dn=cl;
    for(int i=up;i<=dn;i++)for(int j=1;j<=m;j++)if(ma[i][j]=='F'||ma[i][j]=='f'||ma[i][j]=='a')fir++;
    return fir;
}
bool onsky(int x,int y,int &r){
    if(ma[x][y]=='O'||ma[x][y]=='M'){r=y+1;return true;}
    for(int j=y;j<m;j++){
        if((ma[x][j]=='<'||ma[x][j]=='='||ma[x][j]=='R'||ma[x][j]=='D')&&(ma[x][j+1]=='@'||ma[x][j+1]=='|'||ma[x][j+1]=='-'||ma[x][j+1]=='\\'||
        ma[x][j+1]=='/'||ma[x][j+1]=='\''||ma[x][j+1]=='\"'||ma[x][j+1]=='x'||ma[x][j+1]=='V'))return false;
        if(ma[x][j]=='>'||ma[x][j]=='L'){r=j+1;break;}
    }
    return true;
}
void fallfire(){
    for(int i=n-1;i>=2;i--){
        bool move=false;
        for(int j=2;j<m;j++){
            if(ma[i][j]=='F'&&(ma[i+1][j]=='_'||ma[i+1][j]=='x')){
                if(ma[i+1][j]=='_'){
                    move=true;
                    ma[i+1][j]='F';
                    ma[i][j]='_';
                }else ma[i+1][j]='V';
            }
        }
        if(move){
            printmap();
            Sleep(10);
            i+=2;
        }
    }
}
void fallice(){
    for(int i=n-1;i>=2;i--){
        bool move=false;
        for(int j=2;j<m;j++){
            if(ma[i][j]=='<'||ma[i][j]=='O'||ma[i][j]=='R'||ma[i][j]=='M'){
                int f=j,r;
                bool on=onsky(i,j,r);
                if(on){
                    bool check=true;
                    for(int k=f;k<r;k++){
                        if(ma[i+1][k]!='_'&&ma[i+1][k]!='F'&&ma[i+1][k]!='f'&&ma[i+1][k]!='a'){
                            check=false;
                            break;
                        }
                    }
                    if(check){
                        move=true;
                        for(int k=f;k<r;k++){
                            tm[k]=ma[i+1][k];
                            ma[i+1][k]=ma[i][k];
                            ma[i][k]='_';
                        }
                        printmap();
                        Sleep(10);
                        for(int k=f;k<r;k++){
                            if(tm[k]=='F'||tm[k]=='f'||tm[k]=='a'){
                                fire--;
                                if(ma[i+1][k]=='<'||ma[i+1][k]=='='||ma[i+1][k]=='>'||ma[i+1][k]=='O')ma[i+1][k]='_';
                                boom(i+1,k);
                            }else if((ma[i+1][k]=='<'||ma[i+1][k]=='='||ma[i+1][k]=='>'||ma[i+1][k]=='O')&&ma[i+2][k]=='V'){
                                ma[i+1][k]='_';
                                boom(i+1,k);
                            }
                        }
                        for(int k=f;k<r;k++){
                            if(k-1>=f&&ma[i+1][k-1]=='_'&&ma[i+1][k]!='_')ma[i+1][k]=leftlink(ma[i+1][k]);
                            if(k+1<r&&ma[i+1][k+1]=='_'&&ma[i+1][k]!='_')ma[i+1][k]=rightlink(ma[i+1][k]);
                        }
                        printmap();
                        Sleep(10);
                        fallfire();
                    }
                }
            }
        }
        if(move)i+=2;
    }
}
void goice(int x,int y,int pl){
    bool kf=false;
    while((ma[x][y+pl]=='_'||ma[x][y+pl]=='F'||ma[x][y+pl]=='f'||ma[x][y+pl]=='a'||ma[x][y+pl]=='H')&&ma[x+1][y]!='_'&&ma[x+1][y]!='F'&&ma[x+1][y]!='f'&&ma[x+1][y]!='a'){
        if(ma[x+1][y]=='V'){
            ma[x][y]='_';
            boom(x,y);
            kf=true;
        }else if(ma[x][y+pl]=='_'){
            ma[x][y+pl]='O';
            ma[x][y]='_';
            y+=pl;
        }else if(ma[x][y+pl]=='H'){
            kf=true;
            ma[x][y]='_';
            boom(x,y);
            fire--;
            showbmp("data\\boss_hit.bmp",30,180,180,240);
            Sleep(100);
            sprintf(tmp,"data\\boss_%d.bmp",bst);
            showbmp(tmp,30,180,180,240);
        }else{
            char tmp=ma[x][y];
            ma[x][y]=ma[x][y+pl]='_';
            kf=true;
            showbmp(getlib('_'),(y-1)*width,(x+1)*width);
            showbmp(getlib(tmp),(y+pl-1)*width,(x+1)*width);
            Sleep(10);
            boom(x,y+pl);
            fire--;
        }
        printmap();
        Sleep(30);
        if(kf)break;
    }
    if(ma[x+1][y]=='V'){
        ma[x][y]='_';
        boom(x,y);
        kf=true;
    }
    if(kf)fallfire();
    fallice();
}
void gom(int x,int y,int pl){
    if(ma[x+1][y+pl]!='<'&&ma[x+1][y+pl]!='='&&ma[x+1][y+pl]!='>'&&ma[x+1][y+pl]!='O'&&(ma[x][y+pl]=='_'||ma[x][y+pl]=='F'||ma[x][y+pl]=='f'||ma[x][y+pl]=='a')){
        if(ma[x][y+pl]=='_'){
            ma[x][y+pl]='M';
            ma[x][y]='_';
            y+=pl;
            printmap();
            Sleep(20);
        }else{
            ma[x][y+pl]='M';
            ma[x][y]='_';
            showbmp(getlib('_'),(y-1)*width,(x+1)*width);
            showbmp(getlib(ma[x][y+pl]),(y+pl-1)*width,(x+1)*width);
            Sleep(10);
            boom(x,y+pl);
            fire--;
        }
    }else if(ma[x+1][y+pl]=='<'||ma[x+1][y+pl]=='='||ma[x+1][y+pl]=='>'||ma[x+1][y+pl]=='O'){
        while((ma[x][y+pl]=='_'||ma[x][y+pl]=='F'||ma[x][y+pl]=='f'||ma[x][y+pl]=='a')&&ma[x+1][y]!='_'&&ma[x+1][y]!='F'
        &&ma[x+1][y]!='f'&&ma[x+1][y]!='a'){
            char tmp=ma[x][y+pl];
            ma[x][y+pl]='M';
            ma[x][y]='_';
            if(tmp!='_'){
                printmap();
                Sleep(10);
                boom(x,y+pl);
                fallfire();
                fallice();
                fire--;
            }
            y+=pl;
            printmap();
            Sleep(20);
            if(ma[x+1][y]!='<'&&ma[x+1][y]!='='&&ma[x+1][y]!='>'&&ma[x+1][y]!='O')break;
        }
    }
    fallfire();
    fallice();
}
void clear(){for(int i=1;i<=n;i++)for(int j=1;j<=m;j++)showbmp("data\\empty.bmp",(j-1)*30,(i+1)*30);}
bool fall(int mx,int my){
    while(ma[mx+1][my]=='_'||ma[mx+1][my]=='F'||ma[mx+1][my]=='f'||ma[mx+1][my]=='a'||ma[mx+1][my]=='V'){
        printmap();
        Sleep(20);
        if(ma[mx+1][my]=='_'){
            ma[mx+1][my]='B';
            ma[mx++][my]='_';
        }else{
            if(!to)showbmp(addbylvl("data\\player_leftdie",bl),(my-1)*width,(mx+1)*width);
            else showbmp(addbylvl("data\\player_rightdie",bl),(my-1)*width,(mx+1)*width);
            win=0;
            return false;
        }
    }
    fallice();
    fallfire();
    return true;
}
bool crosspipe(int x,int y,int fx,int fy){
    int lx=-1,ly=-1;
    while(ma[x][y]=='|'||ma[x][y]=='-'||ma[x][y]=='\\'||ma[x][y]=='/'||ma[x][y]=='\''||ma[x][y]=='\"'){
        string tm=getlib(ma[x][y]);
        sprintf(tmp,"%s.bmp",string(tm.substr(0,tm.size()-4)+"B").c_str());
        showbmp(tmp,(y-1)*width,(x+1)*width);
        if(lx!=-1&&ly!=-1)showbmp(getlib(ma[lx][ly]),(ly-1)*width,(lx+1)*width);
        int chx[3],chy[3],cnt=0;
        if(ma[x][y]=='|'||ma[x][y]=='/'||ma[x][y]=='\''){chx[++cnt]=x-1,chy[cnt]=y;}
        if(ma[x][y]=='-'||ma[x][y]=='/'||ma[x][y]=='\\'){chx[++cnt]=x;chy[cnt]=y-1;}
        if(ma[x][y]=='|'||ma[x][y]=='\\'||ma[x][y]=='\"'){chx[++cnt]=x+1;chy[cnt]=y;}
        if(ma[x][y]=='-'||ma[x][y]=='\''||ma[x][y]=='\"'){chx[++cnt]=x;chy[cnt]=y+1;}
        for(int i=1;i<=2;i++){
            if(((lx!=-1&&ly!=-1)||(chx[i]!=fx||chy[i]!=fy))&&(chx[i]!=lx||chy[i]!=ly)){
                lx=x;
                ly=y;
                if(chy[i]==y-1)to=0;
                else if(chy[i]==y+1)to=1;
                x=chx[i];
                y=chy[i];
                break;
            }
        }
        Sleep(30);
    }
    showbmp(getlib(ma[lx][ly]),(ly-1)*width,(lx+1)*width);
    if(ma[x][y]=='_'){
        ma[x][y]='B';
        return fall(x,y);
    }else return crosspipe(lx,ly,x,y);
}
void movef(){
    memset(mv,false,sizeof(mv));
    for(int i=1;i<=n;i++){
        for(int j=1;j<=m;j++){
            if((ma[i][j]=='f'||ma[i][j]=='a')&&!mv[i][j]){
                if(ma[i][j]=='f'){
                    if(fx[i][j]&&ma[i][j+1]!='_'&&ma[i][j+1]!='B')fx[i][j]=0;
                    if(!fx[i][j]&&(ma[i][j-1]=='_'||ma[i][j-1]=='B')){
                        if(ma[i][j-1]=='_'){
                            ma[i][j-1]='f';
                            ma[i][j]='_';
                            swap(fx[i][j],fx[i][j-1]);
                            mv[i][j-1]=true;
                        }else{
                            if(!to)showbmp(addbylvl("data\\player_leftdie",bl),(j-2)*width,(i+1)*width);
                            else showbmp(addbylvl("data\\player_rightdie",bl),(j-2)*width,(i+1)*width);
                            win=0;
                            return;
                        }
                    }else if(ma[i][j+1]=='_'||ma[i][j+1]=='B'){
                        if(ma[i][j+1]=='_'){
                            ma[i][j+1]='f';
                            ma[i][j]='_';
                            swap(fx[i][j],fx[i][j+1]);
                            mv[i][j+1]=true;
                        }else{
                            if(!to)showbmp(addbylvl("data\\player_leftdie",bl),j*width,(i+1)*width);
                            else showbmp(addbylvl("data\\player_rightdie",bl),j*width,(i+1)*width);
                            win=0;
                            return;
                        }
                    }
                }else if(ma[i][j]=='a'){
                    if(!fx[i][j]&&ma[i-1][j]!='_'&&ma[i-1][j]!='B')fx[i][j]=1;
                    if(fx[i][j]&&ma[i+1][j]!='_'&&ma[i+1][j]!='B')fx[i][j]=0;
                    if(!fx[i][j]&&(ma[i-1][j]=='_'||ma[i-1][j]=='B')){
                        if(ma[i-1][j]=='_'){
                            ma[i-1][j]='a';
                            ma[i][j]='_';
                            swap(fx[i][j],fx[i-1][j]);
                            mv[i-1][j]=true;
                        }else{
                            if(!to)showbmp(addbylvl("data\\player_leftdie",bl),(j-1)*width,i*width);
                            else showbmp(addbylvl("data\\player_rightdie",bl),(j-1)*width,i*width);
                            win=0;
                            return;
                        }
                    }else if(ma[i+1][j]=='_'||ma[i+1][j]=='B'){
                        if(ma[i+1][j]=='_'){
                            ma[i+1][j]='a';
                            ma[i][j]='_';
                            swap(fx[i][j],fx[i+1][j]);
                            mv[i+1][j]=true;
                        }else{
                            if(!to)showbmp(addbylvl("data\\player_leftdie",bl),(j-1)*width,(i+2)*width);
                            else showbmp(addbylvl("data\\player_rightdie",bl),(j-1)*width,(i+2)*width);
                            win=0;
                            return;
                        }
                    }
                }
            }
        }
    }
    printmap();
}
DWORD WINAPI counttime(LPVOID d){
    printf("按esc暂停\n时间:%02d:%02d:%02d",S,F,M);
    while(true){
        for(int i=1;i<=13;i++)printf("\b");
        printf("时间:%02d:%02d:%02d",S,F,M);
        movef();
        if((re%20==10||(re>=115&&re%10==5))&&re!=130&&re!=150&&!((S*3600+F*60+M)%(re%20==10?10:5))){
            CL--;
            if(CL-24<up)for(int j=1;j<=m;j++)if(ma[CL-24][j]=='F'||ma[CL-24][j]=='f'||ma[CL-24][j]=='a')fire++;
            for(int j=1;j<=m;j++){
                if(CL!=n)ma[CL+1][j]='e';
                if(ma[CL][j]=='B'||ma[CL-1][j]=='B'){
                    int xx=(ma[CL][j]=='B'?CL:CL-1);
                    if(!to)showbmp(addbylvl("data\\player_leftdie",bl),(j-1)*width,(xx+1)*width);
                    else showbmp(addbylvl("data\\player_rightdie",bl),(j-1)*width,(xx+1)*width);
                    win=0;
                    return 0;
                }
                if(ma[CL][j]=='F'||ma[CL][j]=='f'||ma[CL][j]=='a')fire--;
                ma[CL][j]='V';
            }
            printmap();
        }
        if(re==100&&ob){
            int tot=(S*3600+F*60+M)%9;
            if(tot<=2)bst=0;
            else if(tot<=5)bst=tot-2;
            else if(tot==6){
                bst=4;
                queue<int>qx,qy;
                FILE *li=fopen("data\\flist.txt","r");
                int now=1,tot;
                fscanf(li,"%d",&tot);
                for(int i=1;i<=tot+9;i++){
                    if(now<=tot){
                        char s[10];
                        int xx,yy;
                        fscanf(li,"%d%d%s",&xx,&yy,s);
                        sprintf(tmp,"data\\destroy_%s.bmp",s);
                        if(ma[xx][yy]=='B'){
                            if(!to)showbmp(addbylvl("data\\player_leftdie",bl),(yy-1)*width,(xx+1)*width);
                            else showbmp(addbylvl("data\\player_rightdie",bl),(yy-1)*width,(xx+1)*width);
                            win=0;
                            return 0;
                        }else{
                            ma[xx][yy]='_';
                            showbmp(tmp,(yy-1)*width,(xx+1)*width);
                            if(ma[xx][yy-1]=='<'||ma[xx][yy-1]=='='){
                                ma[xx][yy-1]=rightlink(ma[xx][yy-1]);
                                showbmp(getlib(ma[xx][yy-1]),(yy-2)*width,(xx+1)*width);
                            }
                            if(ma[xx][yy+1]=='>'||ma[xx][yy+1]=='='){
                                ma[xx][yy+1]=leftlink(ma[xx][yy+1]);
                                showbmp(getlib(ma[xx][yy+1]),yy*width,(xx+1)*width);
                            }
                        }
                        qx.push(xx);
                        qy.push(yy);
                        now++;
                    }
                    if(now>=11){
                        int xx=qx.front(),yy=qy.front();
                        qx.pop();
                        qy.pop();
                        showbmp("data\\empty.bmp",(yy-1)*width,(xx+1)*width);
                    }
                    Sleep(30);
                }
                fallice();
                fallfire();
                printmap();
                fall(mx,my);
            }
        }
        Sleep(1000);
        M++;
        if(M==60){F++;M=0;}
        if(F==60){S++;F=0;}
    }
    return 0;
}
void CONGRATULATION(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED);
    printf("C");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE);
    printf("O");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_GREEN);
    printf("N");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_BLUE);
    printf("G");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_RED|FOREGROUND_GREEN);
    printf("R");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN);
    printf("A");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    printf("T");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED);
    printf("U");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE);
    printf("L");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN);
    printf("A");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE);
    printf("T");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN);
    printf("I");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_GREEN);
    printf("O");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    printf("N");
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
    printf("S!\n\n");
}
int main(){
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY);
    HWND hwnd=GetConsoleWindow();
    hDC=GetDC(hwnd);
    while(true){
        int ans=0;
        while(ans>4||ans<1){
            system("cls");
            printmid("所罗门之匙2");
            printf("\n\n");
            rr();
            printco('=',wdwd,false);
            printmid("请选择:");
            printf("\n\n"); 
            const char *tmp[4]={"1:选关","2:编辑关卡","3:初始化","4:退出"};
            printsmid(4,tmp);
            printco('=',wdwd,true);
            printf("\n请选择：");
            bool o,t,h,f;
            while(GAKS('1')||GAKS('2')||GAKS('3')||GAKS('4'));
            while(!(GetForegroundWindow()==hwnd&&((o=GAKS('1'))||(t=GAKS('2'))||(h=GAKS('3'))||(f=GAKS('4')))));
            if(o)ans=1;
            else if(t)ans=2;
            else if(h)ans=3;
            else if(f)ans=4;
        }
        if(ans==1){
            re=choslvl();
            system("cls");
            bool res=true;
            if(re!=-1){
                while(res){
                    S=F=M=bst=0;
                    CL=n+1;
                    rdlvlmap(re);
                    res=emp=ob=false;
                    HANDLE h=CreateThread(NULL,0,counttime,NULL,0,NULL);
                    win=-1;
                    to=0;
                    fire=cntfr();
                    while(true){
                        printmap();
                        if(!fire){
                            if(re==100&&!ob){
                                Sleep(20);
                                S=F=M=0;
                                fire=5;
                                FILE *bs=fopen("data\\finalboss.txt","r");
                                fscanf(bs,"%d%d",&n,&m);
                                for(int i=1;i<=n;i++){
                                    fscanf(bs,"%s",&ma[i][1]);
                                    ma[i][m+1]='\0';
                                }
                                ob=emp=true;
                                printmap();
                            }else{
                                win=1;
                                break;
                            }
                        }
                        bool esc,l,r,u,d,z,x;
                        while(GAKS(VK_ESCAPE)||GAKS(VK_LEFT)||GAKS(VK_RIGHT)||GAKS(VK_UP)||GAKS(VK_DOWN)||GAKS('Z')||GAKS('X'));
                        while(!(GetForegroundWindow()==hwnd&&((esc=GAKS(VK_ESCAPE))||(l=GAKS(VK_LEFT))||(r=GAKS(VK_RIGHT))||(u=GAKS(VK_UP))||(d=GAKS(VK_DOWN))||(z=GAKS('Z'))||(x=GAKS('X'))||win==0||fire==0)));
                        if(!win)break;
                        if(!fire)continue;
                        if(esc){
                            clear();
                            TerminateThread(h,0);
                            int ans=0;
                            while(ans>3||ans<1){
                                system("cls");
                                rr();
                                printco('=',wdwd,false);
                                printmid("已暂停。");
                                printf("\n");
                                printmid("请选择：");
                                printf("\n\n");
                                const char *tmp[3]={"1:返回","2:重新开始","3:退出"};
                                printsmid(3,tmp);
                                printco('=',wdwd,true);
                                printf("\n请选择：");
                                bool o,t,h;
                                while(GAKS('1')||GAKS('2')||GAKS('3'));
                                while(!(GetForegroundWindow()==hwnd&&((o=GAKS('1'))||(t=GAKS('2'))||(h=GAKS('3')))));
                                if(o)ans=1;
                                else if(t)ans=2;
                                else if(h)ans=3;
                            }
                            if(ans==1)h=CreateThread(NULL,0,counttime,NULL,0,NULL);
                            else if(ans==2){system("cls");res=true;break;}
                            else if(ans==3){win=-2;break;}
                        }else if(u&&(ma[mx-1][my]=='_'||ma[mx-1][my]=='|'||ma[mx-1][my]=='\\'||ma[mx-1][my]=='\"')){
                            if(ma[mx-1][my]=='_'){
                                if(!to&&(ma[mx-1][my-1]=='_'||ma[mx-1][my-1]=='-'||ma[mx-1][my-1]=='\''||ma[mx-1][my-1]=='\"')&&ma[mx][my-1]!='F'
                                &&ma[mx][my-1]!='f'&&ma[mx][my-1]!='a'&&ma[mx][my-1]!='V'&&ma[mx][my-1]!='_'&&ma[mx][my-1]!='-'&&ma[mx][my-1]!='\"'
                                &&ma[mx][my-1]!='\''&&((ma[mx][my-1]!='O'&&ma[mx][my-1]!='M')||(ma[mx][my-2]!='_'&&ma[mx][my-2]!='F'&&ma[mx][my-2]!='f'))){
                                    if(ma[mx-1][my-1]=='_'){
                                        ma[mx-1][my]='B';
                                        ma[mx--][my]='_';
                                        Sleep(20);
                                        printmap();
                                        ma[mx][my-1]='B';
                                        ma[mx][my--]='_';
                                        Sleep(20);
                                    }else{
                                        ma[mx-1][my]='B';
                                        ma[mx--][my]='_';
                                        Sleep(20);
                                        printmap();
                                        ma[mx][my]='_';
                                        printmap();
                                        if(!crosspipe(mx,my-1,mx,my))break;
                                    }
                                }else if(to&&(ma[mx-1][my+1]=='_'||ma[mx-1][my+1]=='-'||ma[mx-1][my+1]=='/'||ma[mx-1][my+1]=='\\')&&ma[mx][my+1]!='F'&&ma[mx][my+1]!='f'
                                &&ma[mx][my+1]!='a'&&ma[mx][my+1]!='V'&&ma[mx][my+1]!='_'&&ma[mx][my+1]!='-'&&ma[mx][my+1]!='/'
                                &&ma[mx][my+1]!='\\'&&((ma[mx][my+1]!='O'&&ma[mx][my+1]!='M')||(ma[mx][my+2]!='_'&&ma[mx][my+2]!='F'&&ma[mx][my+2]!='f'))){
                                    if(ma[mx-1][my+1]=='_'){
                                        ma[mx-1][my]='B';
                                        ma[mx--][my]='_';
                                        Sleep(20);
                                        printmap();
                                        ma[mx][my+1]='B';
                                        ma[mx][my++]='_';
                                        Sleep(20);
                                    }else{
                                        ma[mx-1][my]='B';
                                        ma[mx--][my]='_';
                                        Sleep(20);
                                        printmap();
                                        ma[mx][my]='_';
                                        printmap();
                                        if(!crosspipe(mx,my+1,mx,my))break;
                                    }
                                }
                            }else{
                                ma[mx][my]='_';
                                printmap();
                                if(!crosspipe(mx-1,my,mx,my))break;
                            }
                        }else if(l){
                            if(to!=0)to=0;
                            else if(ma[mx][my-1]=='_'||ma[mx][my-1]=='O'||ma[mx][my-1]=='M'||ma[mx][my-1]=='-'||ma[mx][my-1]=='\''||ma[mx][my-1]=='\"'){
                                if(ma[mx][my-1]=='_'){
                                    ma[mx][my-1]='B';
                                    ma[mx][my]='_';
                                    if(!fall(mx,my-1))break;
                                }else if(ma[mx][my-1]=='O')goice(mx,my-1,-1);
                                else if(ma[mx][my-1]=='M')gom(mx,my-1,-1);  
                                else{
                                    ma[mx][my]='_';
                                    printmap();
                                    if(!crosspipe(mx,my-1,mx,my))break;
                                }
                                fallice();
                                fallfire();
                                printmap();
                                if(!fall(mx,my))break;
                            }
                        }else if(r){
                            if(to!=1)to=1;
                            else if(ma[mx][my+1]=='_'||ma[mx][my+1]=='O'||ma[mx][my+1]=='M'||ma[mx][my+1]=='-'||ma[mx][my+1]=='\\'||ma[mx][my+1]=='/'){
                                if(ma[mx][my+1]=='_'){
                                    ma[mx][my+1]='B';
                                    ma[mx][my]='_';
                                    if(!fall(mx,my+1))break;
                                }else if(ma[mx][my+1]=='O')goice(mx,my+1,1);
                                else if(ma[mx][my+1]=='M')gom(mx,my+1,1);
                                else{
                                    ma[mx][my]='_';
                                    printmap();
                                    if(!crosspipe(mx,my+1,mx,my))break;
                                }
                                fallice();
                                fallfire();
                                printmap();
                                if(!fall(mx,my))break;
                            }
                        }else if(d&&(ma[mx+1][my]=='|'||ma[mx+1][my]=='/'||ma[mx+1][my]=='\'')){
                            ma[mx][my]='_';
                            printmap();
                            if(!crosspipe(mx+1,my,mx,my))break;
                        }else if(z||x){
                            int pl=(!to?-1:+1);
                            if(ma[mx+1][my+pl]=='_'||ma[mx+1][my+pl]=='O'||ma[mx+1][my+pl]=='<'||ma[mx+1][my+pl]=='='||ma[mx+1][my+pl]=='>'){
                                if(ma[mx+1][my+pl]=='_'){
                                    if(emp)showbmp("data\\makeice_empty1.bmp",(my+pl-1)*width,(mx+2)*width);
                                    else showbmp(addbylvl("data\\makeice_1",bl),(my+pl-1)*width,(mx+2)*width);
                                    Sleep(10);
                                    if(emp)showbmp("data\\makeice_empty2.bmp",(my+pl-1)*width,(mx+2)*width);
                                    else showbmp(addbylvl("data\\makeice_2",bl),(my+pl-1)*width,(mx+2)*width);
                                    Sleep(10);
                                    if(emp)showbmp("data\\makeice_empty3.bmp",(my+pl-1)*width,(mx+2)*width);
                                    else showbmp(addbylvl("data\\makeice_3",bl),(my+pl-1)*width,(mx+2)*width);
                                    Sleep(10);
                                    ma[mx+1][my+pl]='O';
                                    if(ma[mx+1][my+pl-1]!='F'&&ma[mx+1][my+pl-1]!='f'&&ma[mx+1][my+pl-1]!='a'&&ma[mx+1][my+pl-1]!='_'){
                                        ma[mx+1][my+pl]=oleftlink(ma[mx+1][my+pl]);
                                        if(ma[mx+1][my+pl-1]!='@')ma[mx+1][my+pl-1]=orightlink(ma[mx+1][my+pl-1]);
                                    }
                                    if(ma[mx+1][my+pl+1]!='F'&&ma[mx+1][my+pl+1]!='f'&&ma[mx+1][my+pl+1]!='a'&&ma[mx+1][my+pl+1]!='_'){
                                        ma[mx+1][my+pl]=orightlink(ma[mx+1][my+pl]);
                                        if(ma[mx+1][my+pl+1]!='@')ma[mx+1][my+pl+1]=oleftlink(ma[mx+1][my+pl+1]);
                                    }
                                    if(ma[mx+2][my+pl]=='V'){
                                        ma[mx+1][my+pl]='_';
                                        boom(mx+1,my+pl);
                                        if(ma[mx+1][my+pl-1]!='@'&&ma[mx+1][my+pl-1]!='_'&&ma[mx+1][my+pl-1]!='F'&&ma[mx+1][my+pl-1]!='f'&&ma[mx+1][my+pl-1]!='a')
                                            ma[mx+1][my+pl-1]=rightlink(ma[mx+1][my+pl-1]);
                                        if(ma[mx+1][my+pl+1]!='@'&&ma[mx+1][my+pl+1]!='_'&&ma[mx+1][my+pl+1]!='F'&&ma[mx+1][my+pl+1]!='f'&&ma[mx+1][my+pl+1]!='a')
                                            ma[mx+1][my+pl+1]=leftlink(ma[mx+1][my+pl+1]);
                                    }
                                }else{
                                    if(emp)showbmp("data\\makeice_empty3.bmp",(my+pl-1)*width,(mx+2)*width);
                                    else showbmp(addbylvl("data\\makeice_3",bl),(my+pl-1)*width,(mx+2)*width);
                                    Sleep(10);
                                    if(emp)showbmp("data\\makeice_empty2.bmp",(my+pl-1)*width,(mx+2)*width);
                                    else showbmp(addbylvl("data\\makeice_2",bl),(my+pl-1)*width,(mx+2)*width);
                                    Sleep(10);
                                    if(emp)showbmp("data\\makeice_empty1.bmp",(my+pl-1)*width,(mx+2)*width);
                                    else showbmp(addbylvl("data\\makeice_1",bl),(my+pl-1)*width,(mx+2)*width);
                                    Sleep(10);
                                    ma[mx+1][my+pl]='_';
                                    if(ma[mx+1][my+pl-1]!='@'&&ma[mx+1][my+pl-1]!='_'&&ma[mx+1][my+pl-1]!='F'&&ma[mx+1][my+pl-1]!='f'&&ma[mx+1][my+pl-1]!='a')
                                        ma[mx+1][my+pl-1]=rightlink(ma[mx+1][my+pl-1]);
                                    if(ma[mx+1][my+pl+1]!='@'&&ma[mx+1][my+pl+1]!='_'&&ma[mx+1][my+pl+1]!='F'&&ma[mx+1][my+pl+1]!='f'&&ma[mx+1][my+pl+1]!='a')
                                        ma[mx+1][my+pl+1]=leftlink(ma[mx+1][my+pl+1]);
                                    fallice();
                                    fallfire();
                                }
                            }
                            if(!fall(mx,my))break;
                            Sleep(30);
                        }
                    }
                    TerminateThread(h,0);
                    if(res)continue;
                    Sleep(100);
                    clear();
                    system("cls");
                    if(win==1){
                        FILE *save=fopen("data\\save.txt","rb"),*lvl=fopen("data\\level.txt","rb");
                        int pl,tot;
                        fread(&pl,sizeof(int),1,save);
                        printf("你赢了！\n\n");
                        if(!(re%10)&&re!=150&&pl==re)printf("解锁第%d大关！\n\n",bl+1);
                        else if(re==150){
                            printf("通关！\n\n");
                            CONGRATULATION();
                        }
                        fread(&tot,sizeof(int),1,lvl);
                        fclose(save);
                        fclose(lvl);
                        if(re==pl){
                            save=fopen("data\\save.txt","wb");
                            pl++;
                            fwrite(&pl,sizeof(int),1,save);
                            fclose(save);
                        }
                        system("pause");
                    }else if(!win){
                        int ans=0;
                        while(ans>2||ans<1){
                            system("cls");
                            rr();
                            printmid("你输了！");
                            printf("\n\n");
                            printco('=',wdwd,false);
                            printmid("重新开始吗？");
                            printf("\n\n");
                            const char *tmp[2]={"1:是","2:否"};
                            printsmid(2,tmp);
                            printco('=',wdwd,true);
                            printf("\n请选择：");
                            bool o,t;
                            while(GAKS('1')||GAKS('2'));
                            while(!(GetForegroundWindow()==hwnd&&((o=GAKS('1'))||(t=GAKS('2')))));
                            if(o)ans=1;
                            else if(t)ans=2;
                        }
                        if(ans==1){system("cls");res=true;}
                        continue;
                    }
                }
            }
        }else if(ans==2){
            int re=0;
            while(re<1||re>3){
                system("cls");
                rr();
                printco('=',wdwd,false);
                printmid("请选择:");
                printf("\n\n");
                const char *tmp[3]={"1:添加关卡","2:删除关卡","3:退出"};
                printsmid(3,tmp);
                printco('=',wdwd,false);
                printf("\n请选择:");
                bool o,t,h;
                while(GAKS('1')||GAKS('2')||GAKS('3'));
                while(!(GetForegroundWindow()==hwnd&&((o=GAKS('1'))||(t=GAKS('2'))||(h=GAKS('3')))));
                if(o)re=1;
                else if(t)re=2;
                else if(h)re=3;
            }
            if(re==1){
                keybd_event(VK_RETURN,0,0,0);
                keybd_event(VK_RETURN,0,KEYEVENTF_KEYUP,0);
                fflush(stdin);
                FILE *fin=NULL;
                bool die=false;
                int wr=0;
                while(!fin){
                    system("cls");
                    if(wr==2)warning("文件不存在\n");
                    printf("输入back退出\n\n文件路径：");
                    char c[101];
                    scanf("%s",c);
                    if(!strcmp(c,"back")){die=true;break;}
                    fin=fopen(c,"rb");
                    if(!fin)wr=min(wr+1,2);
                    else wr=0;
                }
                if(!die){
                    system("cls");
                    printf("检查文件……");
                    bool flag=true,be=false;
                    char tmp[101];
                    for(int k=1;k<=1;k++){
                        int a,b;
                        if(!~fscanf(fin,"%d%d",&a,&b)){flag=false;break;}
                        if(a>100||b>100||a<1||b<1){flag=false;break;}
                        for(int i=1;i<=a;i++){
                            if(!~fscanf(fin,"%s",tmp+1)){flag=false;break;}
                            if((signed)strlen(tmp+1)!=b){flag=false;break;}
                            for(int j=1;j<=b;j++){
                                if((i==1||i==a||j==1||j==b)&&tmp[j]!='@'){flag=false;break;}
                                if(tmp[j]!='@'&&tmp[j]!='='&&tmp[j]!='>'&&tmp[j]!='<'&&tmp[j]!='O'&&tmp[j]!='B'&&tmp[j]!='F'&&tmp[j]!='_'&&tmp[j]!='f'&&tmp[j]!='|'&&tmp[j]!='L'&&tmp[j]!='D'
                                &&tmp[j]!='-'&&tmp[j]!='\\'&&tmp[j]!='/'&&tmp[j]!='\''&&tmp[j]!='\"'&&tmp[j]!='x'&&tmp[j]!='V'&&tmp[j]!='a'&&tmp[j]!='M'&&tmp[j]!='R'){flag=false;break;}
                                if(tmp[j]=='B')be=true;
                            }
                        }
                    }
                    if(!be)flag=false;
                    if(!flag){
                        printf("失败\n");
                        system("pause");
                    }else{
                        printf("成功\n添加……");
                        FILE *lvl=fopen("data\\level.txt","rb+");
                        int co;
                        fread(&co,sizeof(int),1,lvl);
                        co++;
                        fseek(lvl,0,SEEK_SET);
                        fwrite(&co,sizeof(int),1,lvl);
                        fseek(lvl,0,SEEK_END);
                        fseek(fin,0,SEEK_SET);
                        int a,b;
                        fscanf(fin,"%d%d",&a,&b);
                        int mem=12+a*b;
                        fwrite(&mem,sizeof(int),1,lvl);
                        fwrite(&a,sizeof(int),1,lvl);
                        fwrite(&b,sizeof(int),1,lvl);
                        for(int i=1;i<=a;i++){
                            fscanf(fin,"%s",tmp);
                            fwrite(&tmp,sizeof(char),b,lvl);
                        }
                        fwrite(&mem,sizeof(int),1,lvl);
                        fclose(fin);
                        fclose(lvl);
                        printf("完成\n新关卡为:第%d关\n",co);
                        system("pause");
                    }
                }
            }else if(re==2){
                char buf[10016];
                re=choslvl();
                if(re!=-1){
                    FILE *tmp=fopen("__tmp__tmp__tmp__.txt","wb"),*lvl=fopen("data\\level.txt","rb"),*save=fopen("data\\save.txt","rb");
                    printf("删除……");
                    int tot,pl;
                    fread(&tot,sizeof(int),1,lvl);
                    fread(&pl,sizeof(int),1,save);
                    fclose(save);
                    if(re<pl){
                        pl--;
                        save=fopen("data\\save.txt","wb");
                        fwrite(&pl,sizeof(int),1,save);
                        fclose(save);
                    }
                    tot--;
                    fwrite(&tot,sizeof(int),1,tmp);
                    for(int i=1;i<re;i++){
                        int mem;
                        fread(&mem,sizeof(int),1,lvl);
                        fwrite(&mem,sizeof(int),1,tmp);
                        fread(buf,sizeof(char),mem,lvl);
                        fwrite(buf,sizeof(char),mem,tmp);
                    }
                    int mem;
                    fread(&mem,sizeof(int),1,lvl);
                    fseek(lvl,ftell(lvl)+mem,SEEK_SET);
                    for(int i=re+1;i<=tot+1;i++){
                        int mem;
                        fread(&mem,sizeof(int),1,lvl);
                        fwrite(&mem,sizeof(int),1,tmp);
                        fread(buf,sizeof(char),mem,lvl);
                        fwrite(buf,sizeof(char),mem,tmp);
                    }
                    fclose(lvl);
                    fclose(tmp);
                    system("DEL /a /f data\\level.txt");
                    system("RENAME __tmp__tmp__tmp__.txt level.txt");
                    system("MOVE level.txt data\\level.txt");
                    printf("完成\n\n");
                    system("pause");
                }
            }
        }else if(ans==3){
            int ans=3;
            while(ans<1||ans>2){
                system("cls");
                rr();
                printco('=',wdwd,false);
                printmid("你确定吗？（这将会删除你所有的关卡和存档)");
                printf("\n\n");
                const char *tmp[2]={"1:是","2:否"};
                printsmid(2,tmp);
                printco('=',wdwd,true);
                printf("\n请选择:");
                bool o,t;
                while(GAKS('1')||GAKS('2'));
                while(!(GetForegroundWindow()==hwnd&&((o=GAKS('1'))||(t=GAKS('2')))));
                if(o)ans=1;
                else if(t)ans=2;
            }
            if(ans==1){
                system("cls\nDEL /a /f data\\level.txt");
                FILE *lvl=fopen("data\\level.txt","wb");
                int o=0;
                fwrite(&o,sizeof(int),1,lvl);
                fclose(lvl);
                system("DEL /a /f data\\save.txt");
                FILE *save=fopen("data\\save.txt","wb");
                o=1;
                fwrite(&o,sizeof(int),1,save);
                fclose(save);
                printf("完成\n");
                system("pause"); 
            }
        }else if(ans==4){
            DeleteDC(hDC);
            return 0;
        }
    }
}
