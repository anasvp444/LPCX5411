/*
 * glcd.h
 *
 *  Created on: Dec 15, 2017
 *      Author: user2
 */

#ifndef GLCD_H_
#define GLCD_H_

#define WHITE       0xFFFF
#define SILVER      0xC618
#define BLACK       0x0000
#define GREY        0x8410
#define RED         0xF800
#define MAROON      0x8000
#define FUCHSIA     0xF81F
#define PURPLE      0x8010
#define LIME        0x07E0
#define GREEN       0x0400
#define YELLOW      0xFFE0
#define OLIVE       0x8400
#define BLUE        0x001F
#define NAVY        0x0010
#define AQUA        0x07FF
#define TEAL        0x0410
#define DARK_BLUE   0x0002


#define SPI0_FLEXCOMM   				5      											/* SPI0 (master) is FLEXCOMM 5	*/
#define LPC_SPIMASTERPORT               LPC_SPI0										/* Master SPI port				*/
#define SPI_MASTER_SSEL                 2												/* Master select line			*/
#define LPCMASTERCLOCKRATE				48000000											/* SPI clock rate				*/




void delayms(uint16_t count);
static void SPI_Init_PinMux(void);
static void SPI_setupMaster(void);
static void Glcd_rst_h(void);
static void Glcd_rst_l(void);
static void Glcd_dc_h(void);
static void Glcd_dc_l(void);
static void Glcd_Sent_Data(LPC_SPI_T *pSPI, uint16_t data);
static void Glcd_Write_Cmd(uint16_t cmd);
static void Glcd_Write_Cmd_Data(uint16_t cmd);
void Glcd__Initial(void);
static void Glcd_SetPos(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1);
void Glcd_ClearScreen(uint16_t bColor);
int Glcd_show_font1(uint16_t x, uint16_t y, char chr, uint16_t dcolor, uint16_t bgcolor);
int Glcd_show_font2(uint16_t x, uint16_t y, char chr, uint16_t dcolor, uint16_t bgcolor);
void Glcd_show_image(uint16_t x, uint16_t y, char img1[], uint16_t dcolor,
		uint16_t bgcolor, uint16_t l,uint16_t n);
void Glcd_draw_point(uint16_t val1, uint16_t val2);
void Glcd_ClearScreen_part(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1, uint16_t bColor);



/* Setup master controller */
static void SPI_setupMaster(void)
{
	int ret;
	SPI_CFGSETUP_T spiSetup;
	SPIM_DELAY_CONFIG_T masterDelay;

	/* Initialize SPI controller */
	ret = Chip_SPI_Init(LPC_SPIMASTERPORT);
	LPC_ASSERT(!ret, __FILE__, __LINE__);

	/* Call to initialize first SPI controller for mode0, master mode, MSB first */
	Chip_SPI_Enable(LPC_SPIMASTERPORT);
	spiSetup.master = 1;
	spiSetup.lsbFirst = 0;
	spiSetup.mode = SPI_CLOCK_MODE0;
	Chip_SPI_ConfigureSPI(LPC_SPIMASTERPORT, &spiSetup);

	/* Setup master controller SSELx for active low select */
	Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

	/* Setup master clock rate*/
	Chip_SPIM_SetClockRate(LPC_SPIMASTERPORT, LPCMASTERCLOCKRATE);

	/* Setup master delay (all chip selects) */
	masterDelay.PreDelay = 0x01;
	masterDelay.PostDelay = 0x01;
	masterDelay.FrameDelay = 0x02;
	masterDelay.TransferDelay = 0x01;
	Chip_SPIM_DelayConfig(LPC_SPIMASTERPORT, &masterDelay);
}


static void SPI_Init_PinMux(void)
{

	/* Connect the SPI0 signals to port pins */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 1, 1,   (IOCON_FUNC4 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));	/* SPI0_SSEL2 */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 19,  (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));	/* SPI0_SCK */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 20,  (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));	/* SPI0_MOSI */
	Chip_IOCON_PinMuxSet(LPC_IOCON, 0, 18,  (IOCON_FUNC1 | IOCON_MODE_PULLUP | IOCON_DIGITAL_EN));	/* SPI0_MISO */

	Chip_GPIO_SetPinDIROutput(LPC_GPIO,1, 16);//RST
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 16,0);//RST
	Chip_GPIO_SetPinDIROutput(LPC_GPIO,1, 15);//DC
	Chip_GPIO_SetPinState(LPC_GPIO, 1, 15,0);//DC
}


void delayms(uint16_t count)
{
	uint16_t i,j;

    for(i = 0; i < count; i++)
       {

    	for(j = 0; j < 325; j++);

       }
}


/* Sent data as 8 bit*/
static void Glcd_Sent_Data(LPC_SPI_T *pSPI,uint16_t data)
{
	     pSPI->FIFOWR = (0X77B << 16) | data;
}

/*Reset the TFT*/
static void Glcd_rst_h(void)
{
	 Chip_GPIO_SetPinState(LPC_GPIO, 1, 16, 1);
}

static void Glcd_rst_l(void)
{
	 Chip_GPIO_SetPinState(LPC_GPIO, 1, 16, 0);
}

/*Register select */
static void Glcd_dc_h(void)
{
	 Chip_GPIO_SetPinState(LPC_GPIO, 1, 15, 1);//dc
}

static void Glcd_dc_l(void)
{
	 Chip_GPIO_SetPinState(LPC_GPIO, 1, 15, 0);
}

/*Write command to TFT*/
static void Glcd_Write_Cmd(uint16_t cmd)
{

   	Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);
   	Glcd_dc_l();
   	Glcd_Sent_Data(LPC_SPIMASTERPORT, cmd);
   	Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

}

/*Write  data for command to TFT*/
static void Glcd_Write_Cmd_Data(uint16_t cmd)
{

   	Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);
   	Glcd_dc_h();
   	Glcd_Sent_Data(LPC_SPIMASTERPORT, cmd);
	Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

}


/* Initiate TFT*/
void Glcd_Initial(void)
{
	Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);
	delayms(5);
	Glcd_rst_l();
	delayms(10);
	Glcd_rst_h();
	delayms(120);

	Glcd_Write_Cmd(0x11);
	delayms(120);

	Glcd_Write_Cmd(0xCF);
	Glcd_Write_Cmd_Data(0x00);
	Glcd_Write_Cmd_Data(0xc3);
	Glcd_Write_Cmd_Data(0X30);

	Glcd_Write_Cmd(0xED);
	Glcd_Write_Cmd_Data(0x64);
	Glcd_Write_Cmd_Data(0x03);
	Glcd_Write_Cmd_Data(0X12);
	Glcd_Write_Cmd_Data(0X81);

	Glcd_Write_Cmd(0xE8);
	Glcd_Write_Cmd_Data(0x85);
	Glcd_Write_Cmd_Data(0x10);
	Glcd_Write_Cmd_Data(0x79);

	Glcd_Write_Cmd(0xCB);
	Glcd_Write_Cmd_Data(0x39);
	Glcd_Write_Cmd_Data(0x2C);
	Glcd_Write_Cmd_Data(0x00);
	Glcd_Write_Cmd_Data(0x34);
	Glcd_Write_Cmd_Data(0x02);

	Glcd_Write_Cmd(0xF7);
	Glcd_Write_Cmd_Data(0x20);

	Glcd_Write_Cmd(0xEA);
	Glcd_Write_Cmd_Data(0x00);
	Glcd_Write_Cmd_Data(0x00);

	Glcd_Write_Cmd(0xC0);    //Power control
	Glcd_Write_Cmd_Data(0x22);   //VRH[5:0]

	Glcd_Write_Cmd(0xC1);    //Power control
	Glcd_Write_Cmd_Data(0x11);   //SAP[2:0];BT[3:0]

	Glcd_Write_Cmd(0xC5);    //VCM control
	Glcd_Write_Cmd_Data(0x3d);
    //LCD_DataWrite_ILI9341(0x30);
	Glcd_Write_Cmd_Data(0x20);

	Glcd_Write_Cmd(0xC7);    //VCM control2
    //LCD_DataWrite_ILI9341(0xBD);
	Glcd_Write_Cmd_Data(0xAA); //0xB0

	Glcd_Write_Cmd(0x36);    // Memory Access Control
	Glcd_Write_Cmd_Data(0x08);

	Glcd_Write_Cmd(0x3A);
	Glcd_Write_Cmd_Data(0x55);

	Glcd_Write_Cmd(0xB1);
	Glcd_Write_Cmd_Data(0x00);
	Glcd_Write_Cmd_Data(0x13);

	Glcd_Write_Cmd(0xB6);    // Display Function Control
	Glcd_Write_Cmd_Data(0x0A);
	Glcd_Write_Cmd_Data(0xA2);

	Glcd_Write_Cmd(0xF6);
	Glcd_Write_Cmd_Data(0x01);
	Glcd_Write_Cmd_Data(0x30);

	Glcd_Write_Cmd(0xF2);    // 3Gamma Function Disable
	Glcd_Write_Cmd_Data(0x00);

	Glcd_Write_Cmd(0x26);    //Gamma curve selected
	Glcd_Write_Cmd_Data(0x01);

	Glcd_Write_Cmd(0xE0);    //Set Gamma
	Glcd_Write_Cmd_Data(0x0F);
	Glcd_Write_Cmd_Data(0x3F);
	Glcd_Write_Cmd_Data(0x2F);
	Glcd_Write_Cmd_Data(0x0C);
	Glcd_Write_Cmd_Data(0x10);
	Glcd_Write_Cmd_Data(0x0A);
	Glcd_Write_Cmd_Data(0x53);
	Glcd_Write_Cmd_Data(0XD5);
	Glcd_Write_Cmd_Data(0x40);
	Glcd_Write_Cmd_Data(0x0A);
	Glcd_Write_Cmd_Data(0x13);
	Glcd_Write_Cmd_Data(0x03);
	Glcd_Write_Cmd_Data(0x08);
	Glcd_Write_Cmd_Data(0x03);
	Glcd_Write_Cmd_Data(0x00);

	Glcd_Write_Cmd(0XE1);    //Set Gamma
	Glcd_Write_Cmd_Data(0x00);
	Glcd_Write_Cmd(0x00);
	Glcd_Write_Cmd_Data(0x10);
	Glcd_Write_Cmd_Data(0x03);
	Glcd_Write_Cmd_Data(0x0F);
	Glcd_Write_Cmd_Data(0x05);
	Glcd_Write_Cmd_Data(0x2C);
	Glcd_Write_Cmd_Data(0xA2);
	Glcd_Write_Cmd_Data(0x3F);
	Glcd_Write_Cmd_Data(0x05);
	Glcd_Write_Cmd_Data(0x0E);
	Glcd_Write_Cmd_Data(0x0C);
	Glcd_Write_Cmd_Data(0x37);
	Glcd_Write_Cmd_Data(0x3C);
	Glcd_Write_Cmd_Data(0x0F);

 	Glcd_Write_Cmd(0x11);    //Exit Sleep
	delayms(120);
 	Glcd_Write_Cmd(0x29);    //Display on
	delayms(50);
	Glcd_ClearScreen(AQUA);
}

/*LCD position set*/

static void Glcd_SetPos(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1)
{
	    uint16_t YSH, YSL, YEH, YEL;
        /*Convert 8 bit */
		YSH=y0 >> 8;
		YSL=y0;

		YEH=y1 >> 8;
		YEL=y1;

	 	Glcd_Write_Cmd(0x2A);
		Glcd_Write_Cmd_Data (0x00);
		Glcd_Write_Cmd_Data (x0);
		Glcd_Write_Cmd_Data (0x00);
		Glcd_Write_Cmd_Data (x1);
		Glcd_Write_Cmd(0x2B);

		Glcd_Write_Cmd_Data (YSH);
		Glcd_Write_Cmd_Data (YSL);
		Glcd_Write_Cmd_Data (YEH);
		Glcd_Write_Cmd_Data (YEL);

		/* Write Data */
		Glcd_Write_Cmd(0x2C);
}

/*clear whole screen  */
void Glcd_ClearScreen(uint16_t bColor)
{
	uint16_t i, j;
	uint16_t SH, SL;

	Glcd_SetPos(0, 239, 0, 319);

	Glcd_dc_h();

	Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

	for (i=0; i<320; i++)
	{

	   for (j=0; j<240; j++)
	   {
			SH=bColor >> 8;
			SL=bColor;
			LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
			LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
	   }

	}

	Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);
}


/*Clear part of  screen  */
void Glcd_ClearScreen_part(uint16_t x0, uint16_t x1, uint16_t y0, uint16_t y1, uint16_t bColor)
{
	uint16_t i,j;

	 Glcd_SetPos(x0, x1, y0, y1);

	 Glcd_dc_h();
	 Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

	 for (i = 0; i < (x1-x0); i++)
		{

		   for (j = 0; j < (y1-y0); j++)
			   LPC_SPIMASTERPORT->FIFOWR = (0XF7B << 16) | bColor;
		}

	 Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);
}


/*Show small font*/
int Glcd_show_font1(uint16_t x,uint16_t y,char chr,uint16_t dcolor,uint16_t bgcolor)
{
	uint16_t i, j;
	uint16_t total, width, height, num, addr;
	uint16_t SH, SL;

	char *temp = font1;
	num = chr-32; //Convert char to Corresponding integer

	total = arrt[num+1][1]-arrt[num][1];//Find total no of points
	height = 27;//height is fixed to 27
	width = total/height;
	width = (width*8)-1;//Find width,height is fixed to 27
	addr = arrt[num][1]+height;//starting address of given char pointer,height added for extra added space(" ")

    if(chr == ' ')//for (" ") (starting char)
    	{	total = height;
    		width = 7;
    		addr = 0;
    	}

    if(chr == '~')//for ("~") (final char)
    	{	total = height*2;
    		width = 15;
    	}

    temp += addr; //address location of char

    Glcd_SetPos(x, x+width, y, y+height);//Position set
    Glcd_dc_h();

    Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

	for(j = 0; j < total; j++)
	{
		for(i = 0; i < 8; i++)
		{
		 	if((*temp & (1 << (7-i) ) ) != 0)
			{
				SH=dcolor >> 8;
				SL=dcolor;
		 		LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
		 		LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
			}

			else
			{
	 			SH=bgcolor >> 8;
	 			SL=bgcolor;
				LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
				LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
			}

		}

		temp++;
	 }

	 Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

  return(width);//Width used for given char returns
}

/*Show large font*/

int Glcd_show_font2(uint16_t x, uint16_t y, char chr, uint16_t dcolor, uint16_t bgcolor)
{
	uint16_t i, j;
	uint16_t total, width, height, num, addr;
	uint16_t SH, SL;

	char *temp = font_2;
    num = chr-32;//Convert char to Corresponding integer

	total = font_2_dat[num+1][1] - font_2_dat[num][1];//Find total no of points
	height = 49;//height is fixed to 49
	width = total/height;
	width = (width*8) - 1;//Find width

    addr = font_2_dat[num][1] + height;//starting address of given char pointer,height added for extra added space(" ")

    if(chr == ' ')//for (" ") (starting char)
    	{	total = height;
    		width = 7;
			addr = 0;
    	}

    if(chr == '~')//for ("~") (final char)
    	{	total = height*4;
    		width = 31;
    	}

    temp += addr;//address location of char

    Glcd_SetPos(x, x+width, y, y+height);//Position set
    Glcd_dc_h();

    Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

	for(j = 0; j < total; j++)
	{
		for(i = 0; i < 8; i++)
		{
		 	if((*temp & (1 << (7-i) ) ) != 0)
			{
				SH = dcolor >> 8;
				SL = dcolor;
		 		LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
		 		LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
			}
			else
			{
	 			SH = bgcolor >> 8;
	 			SL = bgcolor;
				LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
				LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
			}

		}

		temp++;
	 }

	 Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

  return(width);//width used for given char returns
}


/*  Show small string
 *  Parameters starting position x,starting position y,array,foreground color,background color,
 *  change flag-- is need to change every call of same string
 */
void Glcd_show_str(uint16_t x, uint16_t y, char str[], uint16_t dcolor,
		uint16_t bgcolor, uint16_t change)
{
	/*variable change for new write in every call*/
	uint16_t add, cmp, l;
	static uint16_t x_tmp, y_tmp;
	uint8_t flag;
	static char str_sml[30];

	if((x_tmp != x) || (y_tmp != y))//compare the position is same as previous call
	 {
		x_tmp = x;
		y_tmp = y;
		flag = 1;
	 }

	else
		flag = 0;

	cmp=strcmp(str, str_sml);//compare the string is same as previous call

	/*check the position and string is not same as before and change flag is true*/
	if(cmp || flag || change)
	  {
			strcpy(str_sml, str);//copy string as temporary
	        l=strlen(str);//find length of string
	        Glcd_ClearScreen_part(x, x + ((l-1)*3*8) + 8, y, y+28, bgcolor);//clear the part were new string written

			while(*str != '\0')//Iterate while string complete
			{
				add=Glcd_show_font1(x, y, *str, dcolor, bgcolor);//write single char and return width used
				switch(*str)
					{
						    case '&':
						    case '-':
						    case '1':
						    case 'R':
							case 'S':
							case 'U':
							case 'Y':
							case 'Z':
							case 'f':
							case 'r':
							case 't':
							case '{':
							case '}':

							  x += 4;  //add extra space

							break;

							case '"':
							case '#':
							case '/':
							case 'B':
							case 'E':
							case 'F':
							case 'H':
							case 'K':
							case 'N':
							case 'P':
							case '\\':
							case 'T':
							case '_':
							case 'j':
							case '~':

								x += 2; //add extra space

							break;

							case '*':
							case '@':
							case 'A':
							case 'M':
							case 'V':
							case 'X':

								x -= 2;//remove extra space

							break;

							default:

								x += 0;

					}

				x += add;//add extra space
				str++;//increment string pointer
			}

	  }
}

/* Show large string
 * Parameters starting position x,starting position y,array,foreground color,background color,
 * change flag-- is need to change every call of same string
 */
void Glcd_show_str_large(uint16_t x, uint16_t y, char str[], uint16_t dcolor,
		uint16_t bgcolor, uint8_t change)
{
	/*variable change for new write in every call*/
	uint16_t add, cmp, l;
	static uint16_t x_tmp, y_tmp;
	uint8_t flag;
	static char str_lar[25];

	if((x_tmp != x) || (y_tmp != y))//compare the position is same as previous call
	 {
		x_tmp = x;
		y_tmp = y;
		flag = 1;
	 }

	else
		flag = 0;

	cmp = strcmp(str, str_lar);//compare the string is same as previous call

	/*check the position and string is not same as before and change flag is true*/
	if(cmp || flag|| change)
	  {
			strcpy(str_lar, str);//copy string as temporary
			l = strlen(str);  //find length of string
			Glcd_ClearScreen_part(x, x + ((l-1)*4*8) + 8, y, y+50, bgcolor);//clear the part were new string written

			while(*str != '\0')//Iterate while string complete
			{
					add = Glcd_show_font2(x, y, *str, dcolor, bgcolor);//write single char and return width used
					switch(*str)
						{
								case '1':

									x += 5;//add extra space

								break;

								case ' ':
								case '!':
								case '\'':
								case ',':
								case '.':
								case ':':
								case ';':
								case 'I':
								case 'i':
								case 'l':
								case '|':
								case '`':
								case 'G':
								case 'b':
								case 'd':
								case 'g':
								case 'q':
								case 't':

									x += 3;//add extra space

								break;

								case 'y':
								case 'z':

									x -= 1;//remove extra space

								break;

								case '#':
								case '$':
								case '+':
								case '0':
								case '2':
								case '3':
								case '5':
								case '6':
								case '7':
								case '8':
								case '9':
								case '<':
								case '=':
								case '>':
								case '?':
								case 'F':
								case 'J':
								case 'L':
								case 'P':
								case '^':
								case 'a':
								case 'c':
								case 'e':
								case 'h':
								case 'n':
								case 'p':
								case 's':
								case 'u':
								case '~':
								case 'k':

									x += 1;//add extra space

								break;

								case 'o':
								case 'f':
								case 'v':
								case 'x':
								case '4':

									x -= 2;//remove extra space

								break;

								case 'A':
								case 'r':

									x -= 3;//remove extra space

								break;

								default:

									x += 0;
						}

			    x += add; //add space for next char
				str++;//increment string pointer
			}
	  }
}


/* Show Image
 * Parameters starting position x,starting position y,array,foreground color,background color,
 * width of image ,height of image
 */
void Glcd_show_image(uint16_t x, uint16_t y, char img[], uint16_t dcolor, uint16_t bgcolor,
		uint16_t width, uint16_t height)
{
		uint16_t i, j, total;
		uint16_t SH, SL;
		total = width*(height-1)/8;// total no of points
		width = width-1;//width
		height = height-1;//height

		char *temp = img;

		Glcd_SetPos(x, x+width, y, y+height);//LCD Position sets
		Glcd_dc_h();

		Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

		for(j = 0; j < total; j++)
		{
			for(i = 0; i < 8; i++)
			{
			 	if((*temp & (1 << (7-i) ) ) != 0)
				{
					SH = dcolor >> 8;
					SL = dcolor;
			 		LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
			 		LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
				}

				else
				{
		 			SH = bgcolor >> 8;
		 			SL = bgcolor;
					LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SH;
					LPC_SPIMASTERPORT->FIFOWR = (0X77B << 16) | SL;
				}

			}

			temp++;
		 }

		 Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

}


/*draw 4 pixel point in coordinate*/
void Glcd_draw_point(uint16_t val1, uint16_t val2)
{
	Glcd_SetPos(val1, val1+1, val2, val2+4);
	Glcd_dc_h();
    Chip_SPI_SetCSPolLow(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);

	for(uint16_t i=0; i<4; i++)
		LPC_SPIMASTERPORT->FIFOWR = (0XF7B << 16) | BLACK;

	Chip_SPI_SetCSPolHigh(LPC_SPIMASTERPORT, SPI_MASTER_SSEL);
}


#endif /* GLCD_H_ */
