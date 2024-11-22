#define JPEG_INTERNALS
#include "jinclude.h"
#include "jpeglib.h"
#include "ili9341.h"
#include "jpeg_view.h"

struct jpeg_decompress_struct cinfo;
typedef struct RGB
{
  uint8_t B;
  uint8_t G;
  uint8_t R;
}RGB_typedef;
struct jpeg_error_mgr jerr;

RGB_typedef *RGB_matrix;

uint16_t RGB16PixelColor;
static uint8_t *rowBuff;

static uint8_t jpeg_decode(JFILE *file, uint8_t *rowBuff,
						   int posx, int posy,
						   UINT sizex, UINT sizey,
						   UINT *iw, UINT *ih)
{
	uint32_t line_counter = 0;
	uint32_t i = 0, xc=0, ratio;
	uint8_t offset=1;
	JSAMPROW buffer[2] = {0};

  UINT lcdWidth, lcdHeight;


  buffer[0] = rowBuff;
  lcdWidth = sizex;
  lcdHeight = sizey;

  cinfo.err = jpeg_std_error(&jerr);

  jpeg_create_decompress(&cinfo);

  jpeg_stdio_src (&cinfo, file);


  jpeg_read_header(&cinfo, TRUE);
  if (cinfo.image_width > 6000)
  {
//	  showMessage("Image width exceeds 6000!!!", 5);
	  return 0;
  }

  if (cinfo.image_width > lcdWidth)
    {
  	  ratio=cinfo.image_width/lcdWidth;
  	  cinfo.scale_num=1;
  	  if (ratio <= 8) {
		  cinfo.scale_denom=1;
		  for(int s = 0x8; s > 0x01; s /=2)
		  {
			  if (ratio & s)
			  {
				  cinfo.scale_denom=s;
				  break;
			  }
		  }
  	  }
  	  else
  	  {
  		cinfo.scale_denom=8;
  	  }
    }

  cinfo.dct_method = JDCT_IFAST;

    jpeg_start_decompress(&cinfo);
    if (cinfo.output_width > lcdWidth)
    {
    	offset = cinfo.output_width / lcdWidth;
    	if (cinfo.output_width % lcdWidth > lcdWidth/4) offset++;
    }

    if (posx <0 || posy < 0)
    {
    	posx=(lcdWidth-(cinfo.output_width*(offset-1)/offset))/2;
    	posy=(lcdHeight-(cinfo.output_height*(offset-1)/offset))/2;
    }
    *iw = cinfo.image_width;
    *ih = cinfo.image_height;

    lcdFillRGB(COLOR_BLACK);

    if (posx > 0  && cinfo.output_width/offset < lcdWidth)
	{
    	lcdDrawRect(posx-1, posy-1, cinfo.output_width/offset+2, cinfo.output_height/offset+2, COLOR_WHITE);
	}

    while (cinfo.output_scanline < cinfo.output_height && line_counter < lcdHeight-posy)
    {
    	(void) jpeg_read_scanlines(&cinfo, buffer, 1);
      RGB_matrix=(RGB_typedef*)buffer[0];
      for(i = 0, xc=0; i < cinfo.output_width && xc < (lcdWidth -posx); i+=offset, xc++)
	  {
    	RGB16PixelColor = (uint16_t)
	    (
	     ((RGB_matrix[i].R & 0x00F8) >> 3)|
	     ((RGB_matrix[i].G & 0x00FC) << 3)|
	     ((RGB_matrix[i].B & 0x00F8) << 8)
	    );
    	lcdDrawPixel(xc+posx, line_counter+posy, RGB16PixelColor);

	  }
      for(i=0; i < offset-1 && cinfo.output_scanline < cinfo.output_height; i++)
    	  (void) jpeg_read_scanlines(&cinfo, buffer, 1);


      line_counter++;

    }

    jpeg_finish_decompress(&cinfo);

  jpeg_destroy_decompress(&cinfo);

  return 1;
}

JPGRESULT jpeg_screen_view(char* path, char* fn,
					  int px, int py,
					  UINT sizex, UINT sizey,
					  UINT *iw, UINT *ih)
{
	FIL file;
	FATFS fs;
	JPGRESULT ret;

	char sf[256];

	rowBuff = JMALLOC(2048);

	sprintf(sf, "%s%s", path, fn);

	if (f_mount(&fs, "", 0) != FR_OK)
	{
		ret = JPG_MOUNT_ERROR;
	}
	else if (f_open(&file, sf, FA_READ) != FR_OK)
	{
		ret = JPG_OPEN_ERROR;
	}
	else if (jpeg_decode(&file, rowBuff, px, py, sizex, sizey, iw, ih) != 1)
	{
		f_close(&file);
		ret = JPG_DECODE_ERROR;
	}
	else
	{
		f_close(&file);
		ret = JPG_OK;
	}

//	f_mount(&fs, "", 0);
	JFREE(rowBuff);
	return ret;
}
