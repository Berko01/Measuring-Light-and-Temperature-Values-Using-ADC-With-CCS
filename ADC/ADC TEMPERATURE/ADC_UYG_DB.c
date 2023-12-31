/******************************************************
      PIC18F452 ile Dahili ADC Uygulamas�
PIC PROG/DEKA     : Port B jumper'� LCD konumunda olmal�
*******************************************************/
#include <18f452.h>     // Kullan�lacak denetleyicinin ba�l�k dosyas� tan�t�l�yor.

#device ADC=10  // 10 bitlik ADC kullan�laca�� belirtiliyor.

// Denetleyici konfig�rasyon ayarlar�
#fuses XT,NOWDT,NOPROTECT,NOBROWNOUT,NOLVP,NOPUT,NOWRT,NODEBUG,NOCPD
#use delay (clock=4000000) // Gecikme fonksiyonu i�in kullan�lacak osilat�r frekans� belirtiliyor.

#use fast_io(c) //Port y�nlendirme komutlar� C portu i�in ge�erli
#use fast_io(b) //Port y�nlendirme komutlar� E portu i�in ge�erli
#use fast_io(e) //Port y�nlendirme komutlar� E portu i�in ge�erli

#define use_portb_lcd TRUE   // LCD B portuna ba�l�

#include <flex_lcd8x2.c>   // lcd.c dosyas� tan�t�l�yor


#INT_AD               // ADC �evrimi bitti kesmesi
void ADC_Kesmesi ( )
{
   output_high(pin_c5);  // RC5 ��k��� 1
   delay_ms(100);
   output_low(pin_c5);   // RC5 ��k��� 0
}

unsigned long int bilgi; // ��aretsiz 16 bitlik tam say� tipinde de�i�ken tan�mlan�yor
float sicaklik,gerilim;            // ondal�kl� tipte voltaj isminde de�i�ken tan�t�l�yor

//********** ANA PROGRAM FONKS�YONU*******

void main ( )
{


   set_tris_c(0x00);  // C portu komple ��k��
   set_tris_b(0x00);  // B portu komple ��k��
   set_tris_e(0x0F);  // E portu komple giri�

   output_c(0x00);    // C portu ��k���n� s�f�rla
   output_b(0x00);    // B portu ��k���n� s�f�rla

   setup_adc(adc_clock_div_32);   // ADC clock frekans� fosc/32
   setup_adc_ports(ALL_ANALOG);   // T�m AN giri�leri analog 
   enable_interrupts(INT_AD);     // AD �evrimi bitti kesmesi tan�t�l�yor
   enable_interrupts(GLOBAL);     // T�m kesmeler aktif

   lcd_init();                            // LCD haz�r hale getiriliyor

   printf(lcd_putc,"\fSICAKLIK"); // LCD'ye yaz� yazd�r�l�yor
   printf(lcd_putc,"\n  UYG."); // LCD'ye yaz� yazd�r�l�yor
   delay_ms(1000);

   while(1)   // sonsuz d�ng�
   {
      set_adc_channel(5);   // RE0/AN5 ucundaki sinyal A/D i�lemine tabi tutulacak
      delay_us(20);         // Kanal se�iminde sonra bu bekleme s�resi verilmelidir
      bilgi=read_adc();     // ADC sonucu okunuyor ve bilgi de�i�kenine aktar�l�yor

      //sicaklik=(49.8/1023)*bilgi;   // Dijitale �evirme i�lemine u�rayan sinyalin gerilimi hesaplan�yor((0.01/0.010041)*50=49.8 LM35 Yaz�l�msal Kalibrasyonu)
      gerilim=(0.0048828125*bilgi)*1000;
      sicaklik=(gerilim/10)+2;
      printf(lcd_putc,"\fDigi=%lu",bilgi); // AN5 ucundaki sinyalin dijital kar��l��� LCD'ye aktar�l�yor
      printf(lcd_putc,"\nT=%.1f%cC",sicaklik, 223); // AN5 ucundaki sinyalin gerilim de�eri LCD'ye aktar�l�yor


   }
}

