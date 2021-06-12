#include  <msp430g2553.h> /* MSP430G2553用のヘッダファイル */
#include <stdint.h> /* uint8_t とか定義してあるヘッダファイル */
#define LED GREEN_LED
int t = 0;
int main(void)
{
  WDTCTL = WDTPW +WDTHOLD;       /* ウォッチドッグタイマーを止める*/
  /*----- クロックキャリブレーション -----*/
  if (CALBC1_1MHZ ==0xFF || CALDCO_1MHZ == 0xFF) {
    while(1);
  }
  /* 1Mhz */
  BCSCTL1 = CALBC1_1MHZ;            /* Set range */
  DCOCTL = CALDCO_1MHZ;            /* Set DCO step + modulation */
  /*--------------------------------------------*/
  
  P1DIR |= BIT6 + BIT0; /* P1.0とP1.6を出力に設定 */
  P1OUT &= ~(BIT6 + BIT0); /* 最初は両方のLEDを消灯 */

  CCR0 = 1000; /* たぶんTACTLで設定されるタイマーのカウントMAX値 */
  CCR2 = 1000; /* たぶんTACTLで設定されるタイマーのカウントとのコンペアマッチ */
  TACTL = TASSEL_2 + MC_3 + TAIE;      /* SMCLK, up-down mode */
  CCTL2 = CCIE; /*　タイマー割り込みスタート */
  
  __bis_SR_register(LPM1_bits + GIE);       /* LPM0モード, 割り込み許可 */
}

/*! **************************************************
* @brief タイマー割り込みの処理
******************************************************/
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A(void)
{
  switch( TA0IV ) {
    static int toggle_span;
    case  2:  break;        // CCR1 not used
    //------------------------------------//
    // 2msec interrupt
    //------------------------------------//
    case  4:
    toggle_span++;
    
    if ( toggle_span== 2 ) {
       digitalWrite(LED, HIGH);   // turn the LED on (HIGH is the voltage level)
        }

     if (toggle_span> 500 ) {
       digitalWrite(LED, LOW);   // turn the LED on (HIGH is the voltage level)
     }

     if (toggle_span> 4500 ) {
       toggle_span = 0;
     }

     




    
    break;         // CCR2
    case 10:  break; // overflow
  }
}
