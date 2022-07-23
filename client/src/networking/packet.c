/**
 * Copyright (c) 2021-2022 Sirvoid
 * 
 * This software is released under the MIT License.
 * https://opensource.org/licenses/MIT
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "raylib.h"
#include "packet.h"
#include "world.h"
#include "networkhandler.h"
#include "chat.h"

#define PACKET_STRING_SIZE 64

unsigned char *Packet_data;
int Packet_Lengths[256] = {
    66, //0
    14, //1
    15, //2
    65 //3
};
int PingCalculation_oldTime = 0;

int Packet_GetLength(unsigned char opcode) {
    return Packet_Lengths[opcode];
}

/*-------------------------------------------------------------------------------------------------------*
*-------------------------------------------Packets Readers----------------------------------------------*
*--------------------------------------------------------------------------------------------------------*/
int PacketReader_index = 1;

unsigned char Packet_ReadByte(void) {
    return Packet_data[PacketReader_index++];
}

char Packet_ReadSByte(void) {
    return Packet_data[PacketReader_index++];
}

short Packet_ReadShort(void) {
    short value = (short)(Packet_data[PacketReader_index] << 8 | Packet_data[PacketReader_index + 1]); 
    PacketReader_index += 2;
    return value;
}

unsigned short Packet_ReadUShort(void) {
    unsigned short value = (unsigned short)(Packet_data[PacketReader_index] << 8 | Packet_data[PacketReader_index + 1]); 
    PacketReader_index += 2;
    return value;
}

int Packet_ReadInt(void) {
    int value = (int)(Packet_data[PacketReader_index] << 24 | Packet_data[PacketReader_index + 1] << 16 | Packet_data[PacketReader_index + 2] << 8 | Packet_data[PacketReader_index + 3]); 
    PacketReader_index += 4;
    return value;
}

char *Packet_ReadString(void) {
    char *string = MemAlloc(PACKET_STRING_SIZE + 1);
    
    for(int i = 0; i < PACKET_STRING_SIZE; i++) {
        string[i] = Packet_data[PacketReader_index++];
    }
    
    string[PACKET_STRING_SIZE] = 0;

    return string;
}


unsigned char* Packet_ReadArray(int size) {
    unsigned char *arr = MemAlloc(size);
    memcpy(arr, &Packet_data[PacketReader_index], size);
    PacketReader_index += size;
    return arr;
}

/*-------------------------------------------------------------------------------------------------------*
*-------------------------------------------Packets Writer-----------------------------------------------*
*--------------------------------------------------------------------------------------------------------*/
int PacketWriter_index = 0;

void Packet_WriteByte(unsigned char *packet, unsigned char value) {
    packet[PacketWriter_index++] = value;
}

void Packet_WriteSByte(unsigned char *packet, char value) {
    packet[PacketWriter_index++] = value;
}

void Packet_WriteShort(unsigned char *packet, short value) {
    packet[PacketWriter_index++] = (char)(value >> 8);
	packet[PacketWriter_index++] = (char)(value);
}

void Packet_WriteUShort(unsigned char *packet, unsigned short value) {
    packet[PacketWriter_index++] = (char)(value >> 8);
	packet[PacketWriter_index++] = (char)(value);
}

void Packet_WriteInt(unsigned char *packet, int value) {
    packet[PacketWriter_index++] = (char)(value >> 24);
	packet[PacketWriter_index++] = (char)(value >> 16);
    packet[PacketWriter_index++] = (char)(value >> 8);
    packet[PacketWriter_index++] = (char)(value);
}

void Packet_WriteString(unsigned char *packet, char *string) {
    int length = TextLength(string);
    for(int i = 0; i < PACKET_STRING_SIZE; i++) {
        if(i < length) {
            packet[PacketWriter_index++] = string[i];
        } else {
            packet[PacketWriter_index++] = 0;
        }  
    }
    
}

/*-------------------------------------------------------------------------------------------------------*
*------------------------------------------Packets Received----------------------------------------------*
*--------------------------------------------------------------------------------------------------------*/
int mapLoadingChunkCnt = 0;
int compressedLength = 0;
unsigned char *compressedMap;
void Packet_H_MapInit(void) {

    World_LoadMap();
    
}


void Packet_H_MapChunk(void) {
    
    
}

void Packet_H_SetBlock(void) {
    int BlockID = Packet_ReadByte();
    Vector3 position = (Vector3) { Packet_ReadInt(), Packet_ReadInt(), Packet_ReadInt() };
    World_SetBlock(position, BlockID, false);
}

void Packet_H_SpawnEntity(void) {
    int ID = Packet_ReadUShort();
    int type = Packet_ReadByte();
    int x = Packet_ReadInt();
    int y = Packet_ReadInt();
    int z = Packet_ReadInt();
    World_AddEntity(ID, type, (Vector3) { x / 64.0f, y / 64.0f, z / 64.0f }, (Vector3) {0, 0, 0});
}

void Packet_H_TeleportEntity(void) {
    int ID = Packet_ReadUShort();
    int x = Packet_ReadInt();
    int y = Packet_ReadInt();
    int z = Packet_ReadInt();
    int yaw = Packet_ReadSByte();
    int pitch = Packet_ReadSByte();
    World_TeleportEntity(ID, (Vector3) { x / 64.0f, y / 64.0f, z / 64.0f }, (Vector3) {pitch / 128.0f * PI, yaw / 128.0f * PI, 0});
}

void Packet_H_Message(void) {
    char *message = Packet_ReadString();
    Chat_AddLine(message);
}

/*-------------------------------------------------------------------------------------------------------*
*--------------------------------------------Packets Sent------------------------------------------------*
*--------------------------------------------------------------------------------------------------------*/
unsigned char *Packet_Identification(char version, char *name) {
    PacketWriter_index = 0;
    unsigned char *packet = (unsigned char*)MemAlloc(Packet_Lengths[0]);
    Packet_WriteByte(packet, 0);
    Packet_WriteByte(packet, version);
    Packet_WriteString(packet, name);
    return packet;
}

unsigned char *Packet_SetBlock(unsigned char blockID, Vector3 position) {
    PacketWriter_index = 0;
    unsigned char *packet = (unsigned char*)MemAlloc(Packet_Lengths[1]);
    Packet_WriteByte(packet, 1);
    Packet_WriteByte(packet, blockID);
    Packet_WriteInt(packet, floor(position.x));
    Packet_WriteInt(packet, floor(position.y));
    Packet_WriteInt(packet, floor(position.z));
    return packet;
}

unsigned char *Packet_PlayerPosition(Vector3 position, Vector2 rotation) {
    PacketWriter_index = 0;
    unsigned char *packet = (unsigned char*)MemAlloc(Packet_Lengths[2]);
    Packet_WriteByte(packet, 2);
    Packet_WriteInt(packet, (int)(position.x * 64));
    Packet_WriteInt(packet, (int)(position.y * 64));
    Packet_WriteInt(packet, (int)(position.z * 64));
    Packet_WriteSByte(packet, round(rotation.x / PI * 128));
    Packet_WriteSByte(packet, round(rotation.y / PI * 128));
    return packet;
}

unsigned char *Packet_SendMessage(char *message) {
    PacketWriter_index = 0;
    unsigned char *packet = (unsigned char*)MemAlloc(Packet_Lengths[3]);
    Packet_WriteByte(packet, 3);
    Packet_WriteString(packet, message);
    return packet;
}