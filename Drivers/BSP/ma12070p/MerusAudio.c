//
// MA120x0P ESP32 Driver
//
// Merus Audio - September 2018
// Written by Joergen Kragh Jakobsen, jkj@myrun.dk
//
// Register interface thrugh I2C for MA12070P and MA12040P
//   Support a single amplifier/i2c address
//
//
#if defined USE_MA12070P

#include <stdio.h>
#include <stdint.h>
#include "i2c.h"
#include "include/MerusAudio.h"
#include "include/ma120x0.h"
#include "FreeRTOS.h"

#define MA_NENABLE_IO  CONFIG_MA120X0_NENABLE_PIN
#define MA_NMUTE_IO    CONFIG_MA120X0_NMUTE_PIN
#define MA_NERR_IO     CONFIG_MA120X0_NERR_PIN
#define MA_NCLIP_IO    CONFIG_MA120X0_NCLIP_PIN


static const char* I2C_TAG = "i2c";
#define I2C_CHECK(a, str, ret)  if(!(a)) {                                             \
        ESP_LOGE(I2C_TAG,"%s:%d (%s):%s", __FILE__, __LINE__, __FUNCTION__, str);      \
        return (ret);                                                                   \
        }


#define I2C_MASTER_SCL_IO CONFIG_MA120X0_SCL_PIN  //4   /*!< gpio number for I2C master clock */
#define I2C_MASTER_SDA_IO CONFIG_MA120X0_SDA_PIN  //0
    /*!< gpio number for I2C master data  */
#define I2C_MASTER_NUM I2C_NUM_0
 /*!< I2C port number for master dev */
#define I2C_MASTER_TX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_RX_BUF_DISABLE   0   /*!< I2C master do not need buffer */
#define I2C_MASTER_FREQ_HZ    100000     /*!< I2C master clock frequency */

#define MA120X0_ADDR  0x20  /*!< slave address for MA120X0 amplifier */

#define WRITE_BIT  I2C_MASTER_WRITE /*!< I2C master write */
//#define READ_BIT   I2C_MASTER_READ  /*!< I2C master read */
#define ACK_CHECK_EN   0x1     /*!< I2C master will check ack from slave*/
#define ACK_CHECK_DIS  0x0     /*!< I2C master will not check ack from slave */
#define ACK_VAL    0x0         /*!< I2C ack value */
#define NACK_VAL   0x1         /*!< I2C nack value */

#define AUDIO_I2C_ADDRESS         MA120X0_ADDR //       ((uint16_t)0x34)


uint32_t i2c_read_id()
{
	uint16_t read_value = 0, tmp = 0;
	HAL_StatusTypeDef status = HAL_OK;

	status = HAL_I2C_Mem_Read(&hi2c1, AUDIO_I2C_ADDRESS << 1, (uint16_t)0, I2C_MEMADD_SIZE_8BIT, (uint8_t*)&read_value, 1, 1000);
	tmp = ((uint16_t)(read_value >> 8) & 0x00FF);

	tmp |= ((uint16_t)(read_value << 8)& 0xFF00);

	read_value = tmp;

	return read_value;
}

void setup_ma120x0()
{
   uint8_t res = ma_read_byte_p(MA120X0_ADDR,1,MA_hw_version__a);
   printf("Hardware version: 0x%02x\n",res);

   res = ma_read_byte_p(MA120X0_ADDR,1,0x35);

   //ma_write_byte_p(MA120X0_ADDR,1,MA_i2s_format__a, 8);          // Set i2s left justified, set audio_proc_enable

   set_MA_i2s_format(0);

   //ma_write_byte_p(MA120X0_ADDR,1,0x36, 0b00000001);

   //ma_write_byte_p(MA120X0_ADDR,1,MA_vol_db_master__a, 0x12);    // Set vol_db_master
   //ma_write_byte_p(MA120X0_ADDR, 1, 0x10, 1); // Set sample rate 44.1kHz

   res = ma_read_byte_p(MA120X0_ADDR,1,MA_error__a);
   printf("Errors : 0x%02x\n",res);

   res = ma_read_byte_p(MA120X0_ADDR,1,0x35); // get_MA_audio_in_mode_mon();
   printf("Audio in mode : 0x%02x\n",res);

   printf("Clear errors\n");
   ma_write_byte_p(MA120X0_ADDR,1,0x2D,0x34);
   ma_write_byte_p(MA120X0_ADDR,1,0x2D,0x30);
   printf("MA120x0P init done\n");

   printf("Unmute\n");
}

void i2c_master_init()
{
	printf("*********************************************setup i2c******************\n");
/*
   int i2c_master_port = I2C_MASTER_NUM;
   i2c_config_t conf;
   conf.mode = I2C_MODE_MASTER;
   conf.sda_io_num = I2C_MASTER_SDA_IO;
   conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
   conf.scl_io_num = I2C_MASTER_SCL_IO;
   conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
   conf.master.clk_speed = I2C_MASTER_FREQ_HZ;
   esp_err_t res = i2c_param_config(i2c_master_port, &conf);
   printf("Driver param setup : %d (sda:%d, scl:%d)\n",res,conf.sda_io_num,conf.scl_io_num);
   res = i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
   printf("Driver installed   : %d\n",res);
   res = gpio_set_drive_capability(I2C_MASTER_SDA_IO,0);
   res = gpio_set_drive_capability(I2C_MASTER_SCL_IO,0);
   */
   //MX_I2C1_Init();
   printf("Reduce drive strength\n");

}

uint8_t ma_write(uint16_t address, uint8_t *wbuf, uint8_t n)
{
	return ma_write_p(MA120X0_ADDR, 1, address, wbuf, n);
}

uint8_t ma_write_p( uint8_t i2c_addr,uint8_t prot, uint16_t address, uint8_t *wbuf, uint8_t n)
{

	/*
 bool ack = ACK_VAL;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, i2c_addr<<1 | WRITE_BIT, ACK_CHECK_EN);
  if (prot == 2) {
    i2c_master_write_byte(cmd, (uint8_t)((address&0xff00)>>8), ACK_VAL);
    i2c_master_write_byte(cmd, (uint8_t)(address&0x00ff), ACK_VAL);
  } else
  {
    i2c_master_write_byte(cmd, (uint8_t) address, ACK_VAL);
  }

  for (int i=0 ; i<n ; i++)
  { if (i==n-1) ack = NACK_VAL;
    i2c_master_write_byte(cmd, wbuf[i], ack);
  }
  i2c_master_stop(cmd);
  int ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  if (ret == ESP_FAIL) { return ret; }
  return ESP_OK;
  */

	HAL_StatusTypeDef status;

   if (prot == 2)
   {
	   status = HAL_I2C_Mem_Write(&hi2c1, i2c_addr<<1, address,
	   	                                 I2C_MEMADD_SIZE_16BIT, wbuf, n, HAL_MAX_DELAY);

   }
   else
   {
	   status = HAL_I2C_Mem_Write(&hi2c1, i2c_addr<<1, address,
	   	                                 I2C_MEMADD_SIZE_8BIT, wbuf, n, HAL_MAX_DELAY);
   }

   return 0;

}

uint8_t ma_write_byte(uint16_t address, uint8_t value)
{
	return ma_write_byte_p(MA120X0_ADDR, 1, address, value);
}

uint8_t ma_write_byte_p(uint8_t i2c_addr,uint8_t prot, uint16_t address, uint8_t value)
{
	//printf("%04x %02x\n",address,value);
	/*
  esp_err_t ret=0;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (i2c_addr<<1) | WRITE_BIT , ACK_CHECK_EN);
  if (prot == 2) {
    i2c_master_write_byte(cmd, (uint8_t)((address&0xff00)>>8), ACK_VAL);
    i2c_master_write_byte(cmd, (uint8_t)(address&0x00ff), ACK_VAL);
  } else
  {
    i2c_master_write_byte(cmd, (uint8_t) address, ACK_VAL);
  }
  i2c_master_write_byte(cmd, value, ACK_VAL);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  if (ret == ESP_FAIL) {
     printf("ESP_I2C_WRITE ERROR : %d\n",ret);
	 return ret;
  }
  return ESP_OK;
  */
	HAL_StatusTypeDef status;

   if (prot == 2)
   {
	   status = HAL_I2C_Mem_Write(&hi2c1, i2c_addr<<1, address,
	   	                                 I2C_MEMADD_SIZE_16BIT, &value, 1, HAL_MAX_DELAY);

   }
   else
   {
	   status = HAL_I2C_Mem_Write(&hi2c1, i2c_addr<<1, address,
	   	                                 I2C_MEMADD_SIZE_8BIT, &value, 1, HAL_MAX_DELAY);
   }
	return 0;
}

uint8_t ma_read(uint16_t address, uint8_t *rbuf, uint8_t n)
{
	return ma_read_p(MA120X0_ADDR, 1, address, rbuf, n);
}

uint8_t ma_read_p(uint8_t i2c_addr, uint8_t prot, uint16_t address, uint8_t *rbuf, uint8_t n)
{
	/*
	esp_err_t ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  if (cmd == NULL ) { printf("ERROR handle null\n"); }
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (i2c_addr<<1) | WRITE_BIT, ACK_CHECK_EN);
  if (prot == 2) {
    i2c_master_write_byte(cmd, (uint8_t)((address&0xff00)>>8), ACK_VAL);
    i2c_master_write_byte(cmd, (uint8_t)(address&0x00ff), ACK_VAL);
  } else
  {
    i2c_master_write_byte(cmd, (uint8_t) address, ACK_VAL);
  }
  i2c_master_start(cmd);
  i2c_master_write_byte(cmd, (i2c_addr<<1) | READ_BIT, ACK_CHECK_EN);
  if (n==1) { 
    i2c_master_read_byte(cmd, rbuf,NACK_VAL);
  } else 
  { i2c_master_read(cmd, rbuf, n-1 ,ACK_VAL);
    // for (uint8_t i = 0;i<n;i++)
    // { i2c_master_read_byte(cmd, rbuf++, ACK_VAL); }
    i2c_master_read_byte(cmd, rbuf + n-1 , NACK_VAL);
  }
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 100 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  if (ret == ESP_FAIL) {
      printf("i2c Error read - readback\n");
	  return ESP_FAIL;
  }
  return ret;
  */
	HAL_StatusTypeDef status;

   if (prot == 2)
   {
	   status = HAL_I2C_Mem_Read(
	            &hi2c1,
				i2c_addr<<1,
				address,
				I2C_MEMADD_SIZE_16BIT,
				rbuf,
	            n,
				HAL_MAX_DELAY);
   }
   else
   {
	   status = HAL_I2C_Mem_Read(
	            &hi2c1,
				i2c_addr<<1,
				address,
				I2C_MEMADD_SIZE_8BIT,
				rbuf,
	            n,
				HAL_MAX_DELAY);
   }

		return 0;
}

uint8_t ma_read_byte(uint16_t address)
{
	return ma_read_byte_p(MA120X0_ADDR, 1, address);
}

uint8_t ma_read_byte_p(uint8_t i2c_addr,uint8_t prot, uint16_t address)
{
	/*
  uint8_t value = 0;
  esp_err_t ret;
  i2c_cmd_handle_t cmd = i2c_cmd_link_create();
  i2c_master_start(cmd);								// Send i2c start on bus
  i2c_master_write_byte(cmd, (i2c_addr<<1) | WRITE_BIT, ACK_CHECK_EN );
  if (prot == 2) {
    i2c_master_write_byte(cmd, (uint8_t)((address&0xff00)>>8), ACK_VAL);
    i2c_master_write_byte(cmd, (uint8_t)(address&0x00ff), ACK_VAL);
  } else
  {
    i2c_master_write_byte(cmd, (uint8_t) address, ACK_VAL);
  }
  i2c_master_start(cmd);							    // Repeated start
  i2c_master_write_byte(cmd, (i2c_addr<<1) | READ_BIT, ACK_CHECK_EN);
  i2c_master_read_byte(cmd, &value, NACK_VAL);
  i2c_master_stop(cmd);
  ret = i2c_master_cmd_begin(I2C_MASTER_NUM, cmd, 1000 / portTICK_RATE_MS);
  i2c_cmd_link_delete(cmd);
  if (ret == ESP_FAIL) {
      printf("i2c Error read - readback\n");
	  return ESP_FAIL;
  }

  return value;
  */

	uint8_t value = 0;
	HAL_StatusTypeDef status;

   if (prot == 2)
   {
	   status = HAL_I2C_Mem_Read(
	            &hi2c1,
				i2c_addr<<1,
				address,
				I2C_MEMADD_SIZE_16BIT,
				&value,
	            1,
				HAL_MAX_DELAY);
   }
   else
   {

	   status = HAL_I2C_Mem_Read(
	            &hi2c1,
				i2c_addr<<1,
				address,
				I2C_MEMADD_SIZE_8BIT,
				&value,
	            1,
				HAL_MAX_DELAY);

   }

		return value;

}

#endif
