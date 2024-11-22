#ifndef INC_JPEG_VIEW_H_
#define INC_JPEG_VIEW_H_

typedef enum JPGRESULT {
	JPG_OK,
	JPG_MOUNT_ERROR,
	JPG_OPEN_ERROR,
	JPG_DECODE_ERROR
} JPGRESULT;

JPGRESULT jpeg_screen_view(char* path, char* fn,
					  int px, int py,
					  UINT sizex, UINT sizey,
					  UINT *iw, UINT *ih);


#endif /* INC_JPEG_VIEW_H_ */
