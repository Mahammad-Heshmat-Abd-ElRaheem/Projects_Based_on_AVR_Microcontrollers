/*
 * TWI_int.h
 *
 *  Created on: FEB 1, 2019
 *      Author: Khaled Mohsen
 */

#ifndef TWI_INT_H_
#define TWI_INT_H_

typedef enum{

	StrConTx = 0x08 ,
	RepStrConTx = 0x10 ,
	SlA_W_Ack = 0x18,
	SLA_W_NO_Ack = 0x20 ,
	Data_TX_Ack = 0x28 ,
	Data_TX_NO_Ack = 0x30 ,
	SLA_W_Arb_Loss = 0x38 ,
	SLA_R_W_ACK = 0x40 ,
	SLA_R_NO_Ack = 0x48 ,
	Data_R_Ack = 0x50 ,
	Data_R_NO_Ack = 0x58 ,
	Own_SLA_W_Ack = 0x60 ,
	Arb_Loss_SLA_RW_Own_SLA_W_Ack = 0x68 ,
	Gen_Call_R_Ack = 0x70 ,
	Arb_Loss_SLA_RW_Gen_Call_R_Ack = 0x78 ,
	Prev_Own_SLA_W_Data_R_Not_Ack = 0x80


}tenuI2C_Check;

//tenuI2C_Check Copy_Status ;

#define True 		1
#define False		0

void TWI_voidInit(void);
void TWI_voidSetMyAddress(u8 Copy_MyAdd);
void TWI_voidSendStart(void);
void TWI_voidSendStop(void);
void TWI_voidSendByte(u8 Copy_Byte);
u8 TWI_u8ReceiveByteWithNoAck(void);
u8 TWI_u8CheckStatus(tenuI2C_Check Copy_Status);

#endif /* TWI_INT_H_ */
