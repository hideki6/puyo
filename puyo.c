/*
  853086 石橋秀規
  基礎プログラミンング演習Ⅱ
  最終課題
  玉が２個ずつ落ちてきて、同じ色が４つ以上揃うと消える。
  急速落下
*/

#include<stdio.h>
#include<stdlib.h>
#include<handy.h>
#include<time.h>

#define WIDTH 550.0//横ウィンドウ
#define LENGTH 750.0//縦ウィンドウ
#define RADIUS 25.0//半径

int field[14][6]={};//縦１４マス、横６マスに分割
int combine[14][6]={};//４つ揃っているかの判定で、既に確認した場所かどうか
int flaglist[80]={};//フラグとして扱う。これが１の時、玉は消える。

int setcolor(int layer, int i);//玉の色を決める
int check(int x, int y, int cfield, int k);//再帰関数 繋がっている玉の個数を数える
void circle(int layer, int x, int y);//玉を描く
void background();//背景を描く
int gameover(int flag, int layer);//ゲームオーバー

int main(){

    int k;//カウンタ変数
    int x, y;//全体のx座標、y座標
    int x1=2, y1=1;//落ちてくる玉（下）の座標
    int x2=2, y2=0;//落ちてくる玉（上）の座標
    int sx1, sy1, sx2, sy2;//キー操作時に用いる、x１、y１、x２、y２の複製
    int color1, color2;//落ちてくる玉の色
    int next1, next2;//次に落ちてくる玉の色
    int num=0;//繋がっている玉の個数
    int state=0;//状態を０〜５で分ける. 0：生成、１：操作と降下、２：全体の落下、３：消えるかどうかの判定、４：消去、５：終了
    double sleep=0.5;//画面を止める時間
    double sleep2=0.5;//sleepを保存する
    int score=0;//スコア
    int key;//キー
    int down;//下カーソルを押したときのカウンタ変数
    int flag=0;//フラグ
    int overflag=0;//ゲームオーバーのフラグ
    hgevent *event;//イベントを取得
    doubleLayer layers;//ダブルレイヤー
    int mainwindow;//windowID
    int layer;//layerID

    mainwindow=HgOpen(WIDTH, LENGTH);//ウィンドウ
    layers=HgWAddDoubleLayer(mainwindow);//レイヤー作成

    HgWSetEventMask(mainwindow,HG_KEY_DOWN);//キー操作

    srandom(time(NULL));
    next1=random()%5+1;//落ちてくる玉（下）の色を決める
    next2=random()%5+1;//落ちてくる玉（上）の色を決める

    background();//背景を描く
    HgSetFillColor(HG_WHITE);
    HgBoxFill(75, 100, 250, 250, 0);
    HgSetFont(HG_M, 20);
    HgSetColor(HG_BLACK);
    HgText(80, 250, " 同じ色の玉が4つ以上揃う");
    HgText(100, 230, "と消えます");
    HgText(80, 200, "消えた個数✖️100が");
    HgText(100, 180, "スコアにプラスされます");
    HgSetFont(HG_HBI, 30);
    HgSetColor(HG_BLACK);
    HgText(100, 300, "説明");
    HgSetColor(HG_WHITE);
    HgText(53, 400, "spaceキーでスタート");

    for(;;){
        key=HgGetChar();//キーを取得
        if(key==' '){//スペースキーを押すと、ループを抜ける
            HgSetFillColor(HG_BLACK);
            HgBoxFill(55, 100, 287, 360, 0);
            break;
        }
    }

    while(1){
        if(state==0){
            x1=2; y1=1;//落ちてくる玉（下）の座標を初期化
            x2=2; y2=0;//落ちてくる玉の（上）の座標を初期化
            color1=next1;//落ちてくる玉（下）の色を決める
            color2=next2;//落ちてくる玉（上）の色を決める
            next1=random()%5+1;//次に落ちる玉（下）の色を決める
            next2=random()%5+1;//次に落ちる玉（上）の色を決める
            sleep=sleep2;
            sleep-=0.005;
            sleep2=sleep;
            down=0;
            state=1;
        }

        if(state==1){//キー操作と落下
        event = HgEventNonBlocking();//キー入力
            flag=0;
            sx1=x1; sy1=y1;
            sx2=x2; sy2=y2;
            if(event!=NULL){
                switch(event->ch) {

                case 's'://一時停止
                    for(;;){
                        key=HgGetChar();
                        if(key=='s'){//もう一度押すと、再開
                            break;
                        }else if(key=='e'){//終了する
                            state=5;
                            break;
                        }
                    }
                    break;
                case'e'://終了する
                    state=5;
                    break;

                case ' '://回転
                    if(sx2>sx1){
                        sx2=sx1; sy2=sy1+1;
                    }else if(sx2<sx1){
                        sx2=sx1; sy2=sy1-1;
                    }else if(sy2>sy1 && sx2!=0){
                        sy2=sy1; sx2=sx1-1;
                    }else if(sx2!=5){
                        sy2=sy1; sx2=sx1+1;
                    }
                    break;
                case HG_R_ARROW://右カーソル、右に移動
                    if(sx1<5 && sx2<5){
                        sx1++; sx2++;
                    }
                    break;
                case HG_L_ARROW://左カーソル、左に移動
                    if(sx1>0 && sx2>0){
                        sx1--; sx2--;
                    }
                    break;
                case HG_D_ARROW://下カーソル、下に移動
                    if(sy1<12 && sy2<12 && down==0){
                        sleep2=sleep;
                        sleep=0.05;
                        down++;
                    }
                    break;
                }
            }else{//それ以外は１つ下に落ちる
                sy1++; sy2++;
                flag=1;
            }
            if(field[sy1][sx1]==0 && field[sy2][sx2]==0 && sy1<14 && sy2<14){
                x1=sx1; y1=sy1;
                x2=sx2; y2=sy2;
            }else if(flag==1){
                field[y1][x1]=color1;
                field[y2][x2]=color2;
                state=2;
            }
        }

        if(state==2){//全て調べて、空白があれば落とす。
            flag=0;
            for(y=12; y>=0; y--){
                for(x=0; x<6; x++){
                    if(field[y][x]!=0 && field[y+1][x]==0){
                        field[y+1][x]=field[y][x];
                        field[y][x]=0;
                        flag=1;
                    }
                }
            }
            if(flag==0) state=3;
        }

        if(state==3){//４つ以上揃っているかどうか調べて、揃っていたら消す。
            flag=0;
            for(y=0; y<14; y++)for(x=0; x<6; x++) combine[y][x]=0;//初期化
            for(k=0; k<80; k++) flaglist[k]=0;//初期化
            k=0;
            for(y=13; y>=0; y--){
                for(x=0; x<6; x++){
                    if(combine[y][x]==0 && field[y][x]!=0){//未確認で空白以外の時
                        k++;
                        num=check(x, y, field[y][x], k);//繋がっている玉の個数を求め、combineにkを入れる
                        if(num>=4){//４個以上繋がっっている時
                            flag=1;
                            flaglist[k]=1;
                            score+=num*100;
                        }
                    }
                }
            }
            if(flag==1){//フラグが立っている場合
                for(y=13; y>=0; y--){
                    for(x=0; x<6; x++){
                        if(flaglist[combine[y][x]]) field[y][x]=6;//fieldを６にする
                    }
                }
                state=4;//次は玉を消す
            }else{
                for(y=0; y<2; y++){
                    for(x=0; x<6; x++){
                        if(field[y][x]!=0) overflag=1;
                    }
                }
                state=0;//新しい玉を出す
            }
        }

        if(state==4){//玉を消す
            HgSleep(0.2);
            for(y=13; y>=0; y--){
                for(x=0; x<6; x++){
                    if(field[y][x]==6) field[y][x]=0;
                }
            }
            state=2;//落下
        }

        HgWSetFont(layer, HG_HBI, 30);
        HgWText(layer, 400, 520, "score");//スコアを描く
        HgWText(layer, 400, 490, "%d", score);
        for(y=0; y<14; y++){
            for(x=0; x<6; x++){
                if(field[y][x]!=0) {
                    setcolor(layer, field[y][x]);//色を決める
                    circle(layer, x, y);//円を描く
                }
            }
        }

        setcolor(layer, next1);
        HgWCircleFill(layer, 450, 625, 25, 1);
        setcolor(layer, next2);
        HgWCircleFill(layer, 450, 675, 25, 1);

        if(state==1){//落ちてくる玉の描写
            setcolor(layer, color1);
            circle(layer, x1, y1-1);
            setcolor(layer, color2);
            circle(layer, x2, y2-1);
        }

        if(gameover(overflag, layer)==1){//ゲームオーバー時
            overflag=0;
            layer=HgLSwitch(&layers);//レイヤー入れ替え
            key=HgGetChar();//キー取得
            if(key=='r'){//rを押すと再開
                for(y=0; y<14; y++){
                    for(x=0; x<6; x++){
                        field[y][x]=0;//初期化
                    }
                }
                state=0;
                sleep=0.5;
            }else if(key=='e'){//eを押すと終了
                break;
            }
        }

        if(state==5) break;//終了

        layer=HgLSwitch(&layers);//レイヤーを入れ替える
        HgLClear(layer);//レイヤー消す
        HgSleep(sleep);//待つ

    }
}

int setcolor(int layer, int i)//玉の色を決める
{
    switch(i){
    case 1:
        return HgWSetFillColor(layer, HG_RED);
        break;
    case 2:
        return HgWSetFillColor(layer, HG_BLUE);
        break;
    case 3:
        return HgWSetFillColor(layer, HG_GREEN);
        break;
    case 4:
        return HgWSetFillColor(layer, HG_YELLOW);
        break;
    case 5:
        return HgWSetFillColor(layer, HG_PURPLE);
        break;
    default:
        return 0;
        break;
    }
}

int check(int x, int y, int cfield, int k)//再帰関数 繋がっている玉の個数を数える
{
    if(field[y][x]!=cfield || combine[y][x]!=0) return 0;//色が違う、または確認済みなら、0を返す。
    int n=1;
    combine[y][x]=k;
    if(y>=0) n+=check(x, y-1, cfield, k);
    n+=check(x+1, y, cfield, k);
    n+=check(x, y+1, cfield, k);
    n+=check(x-1, y, cfield, k);
    return n;
}

void circle(int layer, int x, int y)//玉を描く
{
    HgWSetWidth(layer, 1);
    HgWCircleFill(layer, (x+1)*50+25, (14-y)*50+25, RADIUS, 1);
}

void background()//背景を描く
{
//背景
    HgSetWidth(3);
    HgSetColor(HG_WHITE);
    HgSetFillColor(HG_DGRAY);
    HgBoxFill(0, 0, 50, 750, 0);
    HgBoxFill(50, 0, 450, 50, 0);
    HgBoxFill(500, 0, 50, 750, 0);
    HgBoxFill(350, 50, 50, 700, 0);
    HgBoxFill(400, 700, 100, 50, 0);
    HgBoxFill(400, 550, 100, 50, 0);
    HgBoxFill(400, 470, 100, 20, 0);
    HgBoxFill(400, 270, 100, 20, 0);
    HgSetFillColor(HG_BLACK);
    HgBoxFill(50, 50, 300, 750, 1);
    HgSetColor(HG_RED);
    HgSetWidth(5);
    HgLine(50, 650, 350, 650);
    HgSetColor(HG_BLACK);
    HgSetFont(HG_HBI, 30);
    HgText(400, 520, "score");
//操作方法
    HgSetFont(HG_HBI, 20);
    HgText(410, 430, "操作方法");
    HgSetColor(HG_DBLUE);
    HgSetFont(HG_TBI, 20);
    HgText(400, 390, "→ :右移動");
    HgText(400, 360, "← :左移動");
    HgText(400, 330, " ↓  :加速");
    HgText(400, 300, "space :回転");
//一時停止or再開と、終了
    HgSetColor(HG_DGREEN);
    HgText(400, 230, "一時停止/ ");
    HgText(400, 210, "再開：『s』");
    HgText(400, 170, "終了：『e』");
}

int gameover(int flag, int layer)//ゲームオーバー
{
    if(flag==1){
        HgWSetFillColor(layer, HG_BLACK);
        HgWBoxFill(layer, 50, 400, 300, 160, 0);
        HgWSetFont(layer, HG_M,80);
        HgWSetColor(layer, HG_RED);
        HgWText(layer, 85, 480, "GAME");
        HgWText(layer, 85, 400, "OVER");
        HgWSetFillColor(layer, HG_WHITE);
        HgWBoxFill(layer, 100, 250, 200, 100, 0);
        HgWSetColor(layer, HG_DBLUE);
        HgWSetFont(layer, HG_TBI, 25);
        HgWText(layer, 110, 310, "もう一回...「r」");
        HgWText(layer, 110, 260, "やめる...「e」");
        HgWSetColor(layer, HG_BLACK);
        return 1;
    }else{
        return 0;
    }
}
