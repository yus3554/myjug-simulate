#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(int argc, char *argv[]){
  FILE *outputfile;
  int setting = 0, looptimes = 0;

  outputfile = fopen(argv[1], "w");  // ファイルを書き込み用にオープン(開く)
  if (outputfile == NULL) {          // オープンに失敗した場合
    printf("cannot open\n");         // エラーメッセージを出して
    exit(1);                         // 異常終了
  }

  while(1){
    if(setting > 0 && setting <= 6){
      break;
    }
    printf("設定 : ");
    scanf("%d", &setting);
    if(setting <= 0 || setting > 6){
      printf("設定は1~6を入力してください。\n");
    }
  }
  //配列なので-1する
  setting--;

  while(1){
    if(looptimes > 1){
      break;
    }
    printf("回転数 : ");
    scanf("%d", &looptimes);
    if(looptimes < 2){
      printf("回転数は2回転以上を入力してください。\n");
    }
  }


  int i;
  // メダルの枚数
  int medals = 0;

  // ボーナスの合計
  int bbsum = 0, rbsum = 0;

  // 角チェ、ブドウ、リプレイの合計
  int cornerCsum = 0, grapesum = 0, repsum = 0;

  double n;

  //設定差
  // 上からブドウ、角チェ、角チェからのボーナス、角チェからのボーナスのうちビッグ、単独ビッグ、単独バケ
  double grape[6] = {1.0/6.35, 1.0/6.29, 1.0/6.25, 1.0/6.23, 1.0/6.18, 1.0/6.07};
  double cornerC[6] = {1.0/36.03, 1.0/35.95, 1.0/34.69, 1.0/33.51, 1.0/33.40, 1.0/33.23};
  double cornerCbonus[6] = {5.44, 5.65, 5.77, 5.93, 6.22, 6.69};
  double cornerCbig[6] = {45.45, 45.63, 44.95, 44.83, 44.26, 42.42};
  double single_bb[6] = {1.0/402.1, 1.0/397.2, 1.0/383.3, 1.0/372.4, 1.0/352.3, 1.0/334.4};
  double single_rb[6] = {1.0/668.7, 1.0/528.5, 1.0/496.5, 1.0/409.6, 1.0/390.1, 1.0/334.4};

  // 上からリプレイ、ピエロ、ベル、中チェ1、中チェ2
  double rep = 1.0/7.3;
  double piero = 1.0/1024.0;
  double bell = 1.0/1024.0;
  double centerC1 = 1.0/6553.6;
  double centerC2 = 1.0/6553.6;

  // ランダムの初期化
  srand((unsigned)time(NULL));

  fprintf(outputfile, "%d %d\n", 0, medals);

  for(i = 0; i < looptimes; i++){
    medals -= 3;
    n = (double)(rand()%1000000)/1000000.0;
    //重複
    if(n < centerC1){
      bbsum++;
      medals++;
      medals += 312;
      //printf("中チェ1\n");
      //printf("BIG BONUS\n");
    }else if(n < centerC1+centerC2){
      bbsum++;
      medals++;
      medals += 312;
      //printf("中チェ2\n");
      //printf("BIG BONUS\n");
    }else if(n < centerC1+centerC2+cornerC[setting]){
      cornerCsum++;
      medals++;
      //printf("角チェ : 1/%lf\n", (double)(i+1)/(double)cornerCsum);
      n = (double)(rand()%1000000)/1000000.0;
      if(n < cornerCbonus[setting]/100.0){
        n = (double)(rand()%1000000)/1000000.0;
        if(n < cornerCbig[setting]/100.0){
          bbsum++;
          medals += 312;
          //printf("BIG BONUS\n");
        }else{
          rbsum++;
          medals += 104;
          //printf("REG BONUS\n");
        }
      }else{
        //printf("notBONUS\n");
      }
    }else if(n < centerC1+centerC2+cornerC[setting]+piero){
      medals += 10;
      //printf("ピエロ\n");
    }else if(n < centerC1+centerC2+cornerC[setting]+piero+bell){
      medals += 15;
      //printf("ベル\n");
    }else if(n < centerC1+centerC2+cornerC[setting]+piero+bell+grape[setting]){
      grapesum++;
      medals += 7;
      //printf("sum : %lf", sum);
      //printf("ぶどう : 1/%lf\n", (double)(i+1)/(double)grapesum);
    }else if(n < centerC1+centerC2+cornerC[setting]+piero+bell+grape[setting]+rep){
      repsum++;
      medals += 3;
      //printf("リプレイ\n");
    }else if(n < centerC1+centerC2+cornerC[setting]+piero+bell+grape[setting]+rep+single_bb[setting]){
      bbsum++;
      medals += 312;
    }else if(n < centerC1+centerC2+cornerC[setting]+piero+bell+grape[setting]+rep+single_bb[setting]+single_rb[setting]){
      rbsum++;
      medals += 104;
    }else{
        //printf("外れ\n");
    }

    //while(getchar() != '\n'){}

    fprintf(outputfile, "%d %d\n", i+1, medals);
  }

  printf("\n[試行結果]\n");
  printf("設定 \t: %d\n回転数 \t: %d回転\n差枚数 \t: %d枚\n機械割 \t: %.2lf%%\n", setting+1, looptimes, medals, (double)(looptimes*3+medals)/(double)(looptimes*3)*100.0);
  printf("----------------------------------\n");
  printf("角チェ \t: %6d回 1/%.2lf\n", cornerCsum, (double)(looptimes+1)/(double)cornerCsum);
  printf("ぶどう \t: %6d回 1/%.2lf\n", grapesum, (double)(looptimes+1)/(double)grapesum);
  printf("BIG \t: %6d回 1/%.2lf\n", bbsum, (double)(looptimes+1)/(double)bbsum);
  printf("REG \t: %6d回 1/%.2lf\n", rbsum, (double)(looptimes+1)/(double)rbsum);
  printf("合算 \t: %6d回 1/%.2lf\n", bbsum+rbsum, (double)(looptimes+1)/((double)bbsum+(double)rbsum));

  fclose(outputfile);

  return 0;
}
