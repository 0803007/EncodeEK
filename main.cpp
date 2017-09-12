#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/* 
 * Created By Franco Lin on 20170912
 * run this program using the console pauser or add your own getch, system("pause") or input loop 
*/

struct DatagramHeader
{
	long DatagramType;
	struct {
		long LowDateTime;
		long HighDateTime;
	} DateTime;
};
struct ConfigurationHeader
{
	char SurveyName[128]; // ¡¨Loch Ness¡¨
	char TransectName[128];
	char SounderName[128]; // ¡§ER60¡¨
	char version [30];
	char spare [98];
	long TransducerCount; // 1 to 7
};
struct ConfigurationTransducer {
	char ChannelId[128]; // Channel identification
	long BeamType; // 0 = Single, 1 = Split
	float Frequency; // [Hz]
	float Gain; // [dB] - See note below
	float EquivalentBeamAngle; // [dB]
	float BeamWidthAlongship; // [degree]
	float BeamWidthAthwartship; // [degree]
	float AngleSensitivityAlongship;
	float AngleSensitivityAthwartship;
	float AngleOffsetAlongship; // [degree]
	float AngleOffsetAthwartship; // [degree]
	float PosX; // future use
	float PosY; // future use
	float PosZ; // future use
	float DirX; // future use
	float DirY; // future use
	float DirZ; // future use
	float PulseLengthTable[5];
	// Available pulse lengths for the channel [s]
	char Spare1[8]; // future use
	float GainTable[5];
	// Gain for each pulse length in the PulseLengthTable [dB]
	char Spare2[8]; // future use
	float SaCorrectionTable[5];
	// Sa correction for each pulse length in the PulseLengthTable [dB]
	char Spare3[8];
	char GPTSoftwareVersion [16];
	char Spare4[28];
};
struct ConfigurationDatagram {
	DatagramHeader DgHeader ;// ¡¨CON0¡¨
	ConfigurationHeader ConfigHeader;
	ConfigurationTransducer Transducer[];
};

struct TextDatagram{
	DatagramHeader DgHeader; // ¡§NME0¡¨
	char Text[]; // ¡¨$GPGLL,5713.213,N......¡¨
};
struct SampleDatagram
{
	DatagramHeader DgHeader; // ¡§RAW0¡¨
	
	short Channel; // Channel number
	short Mode;	// Datatype: 1 = Power, 2 = Angle, 3 = Both
	float TransducerDepth; // [m]
	float Frequency; // [Hz]
	float TransmitPower; // [W]
	float PulseLength; // [seconds]
	float BandWidth; // [Hz]
	float SampleInterval; // [s]
	float SoundVelocity; // [m/s]
	float AbsorptionCoefficient; // [dB/m]
	float Heave; // [m]
	float Roll; // [degree]
	float Pitch; // [degree]
	float Temperature; // [deg C]
	float Heading; // [degree]
	short TransmitMode;	// 0 = Active, 1 = Passive	// 2 = Test, -1 = Unknown
	char Spare[6];
	long Offset; // First sample
	long Count; // Number of samples
	short Power[]; // Compressed format - See below!
	short Angle[]; // See below!
};

void printDatagramHeader(DatagramHeader *datagramHeader){
	printf("DatagramType: %d\n", &datagramHeader->DatagramType);	
	printf("LowDateTime: %d\n", &datagramHeader->DateTime.LowDateTime);	
	printf("HighDateTime: %d\n", &datagramHeader->DateTime.HighDateTime);	
}
void printConfigurationDatagram(ConfigurationDatagram *cd){
	
	printDatagramHeader(&cd->DgHeader);
	
	printf("SurveyName: %s\n",(unsigned char*)cd->ConfigHeader.SurveyName);	
	printf("TransectName: %s\n",(unsigned char*)cd->ConfigHeader.TransectName);
	printf("SounderName: %s\n",(unsigned char*)cd->ConfigHeader.SounderName);
	printf("version: %s\n",(unsigned char*)cd->ConfigHeader.version);
	printf("spare: %s\n",(unsigned char*)cd->ConfigHeader.spare);
	printf("TransducerCount %d\n",cd->ConfigHeader.TransducerCount);	

	
	for (int i=0;i<cd->ConfigHeader.TransducerCount;i++){
		ConfigurationTransducer *ct = &cd->Transducer[i];
		
		printf("ChannelId: %s\n",(unsigned char*)ct->ChannelId);	
		printf("BeamType:  %d\n", ct->BeamType);
		printf("Frequency: %f\n", ct->Frequency);
		
		printf("Gain: %f\n", ct->Gain);
		printf("EquivalentBeamAngle: %f\n", ct->EquivalentBeamAngle);
		printf("BeamWidthAlongship: %f\n", ct->BeamWidthAlongship);
		printf("BeamWidthAthwartship: %f\n", ct->BeamWidthAthwartship);
		printf("AngleSensitivityAlongship: %f\n", ct->AngleSensitivityAlongship);
		printf("AngleSensitivityAthwartship: %f\n", ct->AngleSensitivityAthwartship);
		printf("AngleOffsetAlongship: %f\n", ct->AngleOffsetAlongship);
		printf("AngleOffsetAthwartship: %f\n", ct->AngleOffsetAthwartship);
		printf("PosX: %f\n", ct->PosX);
		printf("PosY: %f\n", ct->PosY);
		printf("PosZ: %f\n", ct->PosZ);
		printf("DirX: %f\n", ct->DirX);
		printf("DirY: %f\n", ct->DirY);
		printf("DirZ: %f\n", ct->DirZ);
		printf("PulseLengthTable: %f %f %f %f %f\n", ct->PulseLengthTable[0], ct->PulseLengthTable[1], ct->PulseLengthTable[2], ct->PulseLengthTable[3], ct->PulseLengthTable[4], ct->PulseLengthTable[5] );
		printf("Spare1: %s\n",(unsigned char*)ct->Spare1);
		printf("GainTable: %f %f %f %f %f\n", ct->GainTable[0], ct->GainTable[1], ct->GainTable[2], ct->GainTable[3], ct->GainTable[4], ct->GainTable[5] );
		printf("Spare2: %s\n",(unsigned char*)ct->Spare2);
		printf("SaCorrectionTable: %f %f %f %f %f\n", ct->SaCorrectionTable[0], ct->SaCorrectionTable[1], ct->SaCorrectionTable[2], ct->SaCorrectionTable[3], ct->SaCorrectionTable[4], ct->SaCorrectionTable[5] );
		printf("Spare3: %s\n",(unsigned char*)ct->Spare3);
		printf("GPTSoftwareVersion: %s\n",(unsigned char*)ct->GPTSoftwareVersion);
		printf("Spare4: %s\n",(unsigned char*)ct->Spare4);	
	}
	return;
}
void saveDatagramHeader(FILE *fp, DatagramHeader *datagramHeader){	
		
	fprintf(fp,"DatagramType: %d\n", datagramHeader->DatagramType);
	fprintf(fp,"LowDateTime: %d\n",  datagramHeader->DateTime.LowDateTime);	
	fprintf(fp,"HighDateTimeL %d\n", datagramHeader->DateTime.HighDateTime);	
	//fwrite(td->Text, strlen(td->Text),1,fp);
}
void saveConfigurationDatagram(FILE *fp, ConfigurationDatagram *cd){
	
	fprintf(fp,"------------ConfigurationDatagram--------------\n");

	saveDatagramHeader(fp, &cd->DgHeader);
	
	fprintf(fp,"SurveyName: %s\n",  (unsigned char*)cd->ConfigHeader.SurveyName);	
	fprintf(fp,"TransectName: %s\n",(unsigned char*)cd->ConfigHeader.TransectName);
	fprintf(fp,"SounderName: %s\n", (unsigned char*)cd->ConfigHeader.SounderName);
	fprintf(fp,"version: %s\n",		(unsigned char*)cd->ConfigHeader.version);
	fprintf(fp,"spare: %s\n",		(unsigned char*)cd->ConfigHeader.spare);
	fprintf(fp,"TransducerCount %d\n",              cd->ConfigHeader.TransducerCount);	

	for (int i=0;i<cd->ConfigHeader.TransducerCount;i++){
		ConfigurationTransducer *ct = &cd->Transducer[i];
		
		fprintf(fp,"ChannelId: %s\n",(unsigned char*)ct->ChannelId);	
		fprintf(fp,"BeamType: %d\n", ct->BeamType);
		fprintf(fp,"Frequency: %f\n", ct->Frequency);
		fprintf(fp,"Gain: %f\n", ct->Gain);
		fprintf(fp,"EquivalentBeamAngle: %f\n", ct->EquivalentBeamAngle);
		fprintf(fp,"EquivalentBeamAngle: %f\n", ct->BeamWidthAlongship);
		fprintf(fp,"BeamWidthAthwartship: %f\n", ct->BeamWidthAthwartship);
		fprintf(fp,"AngleSensitivityAlongship: %f\n", ct->AngleSensitivityAlongship);
		fprintf(fp,"AngleSensitivityAthwartship: %f\n", ct->AngleSensitivityAthwartship);
		fprintf(fp,"AngleOffsetAlongship: %f\n", ct->AngleOffsetAlongship);
		fprintf(fp,"AngleOffsetAthwartship: %f\n", ct->AngleOffsetAthwartship);
		fprintf(fp,"PosX: %f\n", ct->PosX);
		fprintf(fp,"PosY: %f\n", ct->PosY);
		fprintf(fp,"PosZ: %f\n", ct->PosZ);
		fprintf(fp,"DirX: %f\n", ct->DirX);
		fprintf(fp,"DirY: %f\n", ct->DirY);
		fprintf(fp,"DirZ: %f\n", ct->DirZ);
		fprintf(fp,"PulseLengthTable: %f %f %f %f %f\n", ct->PulseLengthTable[0], ct->PulseLengthTable[1], ct->PulseLengthTable[2], ct->PulseLengthTable[3], ct->PulseLengthTable[4], ct->PulseLengthTable[5] );
		fprintf(fp,"Spare1: %s\n",(unsigned char*)ct->Spare1);
		fprintf(fp,"GainTable: %f %f %f %f %f\n", ct->GainTable[0], ct->GainTable[1], ct->GainTable[2], ct->GainTable[3], ct->GainTable[4], ct->GainTable[5] );
		fprintf(fp,"Spare2: %s\n",(unsigned char*)ct->Spare2);
		fprintf(fp,"SaCorrectionTable: %f %f %f %f %f\n", ct->SaCorrectionTable[0], ct->SaCorrectionTable[1], ct->SaCorrectionTable[2], ct->SaCorrectionTable[3], ct->SaCorrectionTable[4], ct->SaCorrectionTable[5] );
		fprintf(fp,"Spare3: %s\n",(unsigned char*)ct->Spare3);
		fprintf(fp,"GPTSoftwareVersion: %s\n",(unsigned char*)ct->GPTSoftwareVersion);
		fprintf(fp,"Spare4: %s\n",(unsigned char*)ct->Spare4);	
	}

	return;
}
void saveSampleDatagram(FILE *fp, SampleDatagram *sd){
	fprintf(fp,"------------SampleDatagram--------------\n");
	
	saveDatagramHeader(fp, &sd->DgHeader);
	
	fprintf(fp,"Channel: %d\n", sd->Channel);
	fprintf(fp,"Mode:%d\n", sd->Mode);
	fprintf(fp,"TransducerDepth: %f\n", sd->TransducerDepth);
	fprintf(fp,"Frequency: %f\n", sd->Frequency);
	fprintf(fp,"TransmitPower: %f\n", sd->TransmitPower);
	fprintf(fp,"PulseLength: %f\n", sd->PulseLength);
	fprintf(fp,"BandWidth: %f\n", sd->BandWidth);
	fprintf(fp,"SampleInterval: %f\n", sd->SampleInterval);
	fprintf(fp,"SoundVelocity: %f\n", sd->SoundVelocity);
	fprintf(fp,"AbsorptionCoefficient: %f\n", sd->AbsorptionCoefficient);
	fprintf(fp,"Heave:%f\n", sd->Heave);
	fprintf(fp,"Roll: %f\n", sd->Roll);
	fprintf(fp,"Pitch: %f\n", sd->Pitch);
	fprintf(fp,"Temperature: %f\n", sd->Temperature);
	fprintf(fp,"Heading: %f\n", sd->Heading);
	fprintf(fp,"TransmitMode: %d\n", sd->TransmitMode);
	fprintf(fp,"Spare: %d\n", sd->Spare);
	fprintf(fp,"Offset: %d\n", sd->Offset);
	fprintf(fp,"Count: %d\n", sd->Count);	
	
	fprintf(fp,"Power: ");
	for (int i=0;i<sd->Count;i++)
		fprintf(fp,"%d ", sd->Power[i]);
	
	fprintf(fp,"\n");
	fprintf(fp,"Angle: ");
	for (int i=0;i<sd->Count;i++)
		fprintf(fp,"%d ", sd->Angle[i]);
		
	fprintf(fp,"\n");
	
	return;
}
void saveTextDatagram(FILE *fp, TextDatagram *td){
	fprintf(fp,"------------TextDatagram--------------\n");
	fwrite(td->Text, strlen(td->Text),1,fp);
	fwrite("\n",1,1,fp);
	
	return;
}

int main(int argc, char** argv) {
	
	long Length;
	long Length2;
	long Length3;
	long Length4;
	long Length5;
	long Length6;
	long Length7;
	long Length8;
	long Length9;
	
	
	short Channel; // Channel number
	short Mode; // Datatyp
	float TransducerDepth; // [m]
	float Frequency; // [Hz]
	float TransmitPower;
	float PulseLength; // [s]
	float BandWidth; // [Hz]
	float SampleInterval; // [s]	
	float SoundVelocity; // [m/s]
	float AbsorptionCoefficient; // [dB/m]
	float Heave; // [m]
	float TxRoll; // [deg]
	float TxPitch; // [deg]
	float Temperature;	
	short Spare1;
	short Spare2;
	float RxRoll;
	float RxPitch;
	long Offset; // First sample
	long Count; // Number of samples		
	
	static char Text[40-28]; 
	static char Text2[31336];
	
	if(argc < 3 || argc > 4)
	{ 
		printf("The correct format is: EncodeEK input output \n");
		exit(1);
	}
	
	
    //FILE *file = fopen("C:\\work\\Dev-Cpp-5.11-TDM-GCC-x64 4.9.2 Portable\\OR3\\D20170509-T232204.raw", "rb");
    //FILE *output = fopen("C:\\work\\Dev-Cpp-5.11-TDM-GCC-x64 4.9.2 Portable\\OR3\\output2.raw","w");
    FILE *file = fopen(argv[1], "rb");
    FILE *output = fopen(argv[2],"w");
    if(!file) { 
        puts("Can't Read File"); 
        return 1; 
    } 
     
    
    //----------------Configuration---------------------------- 
    
    //fread(&Length,sizeof(long),1,file); 
    fread(&Length,sizeof(long),1,file); 
   
    ConfigurationDatagram *cd = (ConfigurationDatagram *)malloc(Length);
    
    fread(cd,Length,1,file);

    saveConfigurationDatagram(output, cd);

	
	fread(&Length2,sizeof(long),1,file); 
	
	//--------------------- (NMEA)--------------------- 
	/*
	fread(&Length3,sizeof(long),1,file); 
	
	TextDatagram *td_nmea = (TextDatagram *)malloc(Length3);
	fread(td_nmea,Length3,1,file);
	 

	fread(&Length4,sizeof(long),1,file); 	*/
	//------------------(Annotation)------------------ 
	/*
	fread(&Length5,sizeof(long),1,file); 
	//DatagramHeader j
    fread(&datagramHeader3.DatagramType,sizeof(long),1,file);    
	fread(&datagramHeader3.DateTime.LowDateTime,sizeof(long),1,file);    
	fread(&datagramHeader3.DateTime.HighDateTime,sizeof(long),1,file);
	
	fread(Text2,sizeof(char),31336-12,file);
	
	fread(&Length6,sizeof(long),1,file); */
	
	//------------------(Sample)------------------
	/*
	fread(&Length7,sizeof(long),1,file); 
	fread(Text2,sizeof(char),31336,file); 
	fread(&Length8,sizeof(long),1,file); 
	*/
	
	printf("-------Configuratio Length: %d-------\n", Length);
	//printConfigurationDatagram(&cd->DgHeader, &cd->ConfigHeader, *cd->Transducer);
	printConfigurationDatagram(cd);
	printf("-------Configuratio Length End: %d------\n", Length2);
	printf("Proccessing!\n");
	

    long len,len2;
    
	//for (int i=0;i<10;i++){
	while( !feof(file)){
		fread(&len,sizeof(long),1,file); 
		char text[len];
	
		fread(text,sizeof(char),len,file);
				
		//determine Sample OR NMEA  from the 13's Byte if NMEA's first char "$" 				
		if (text[12] != '$'){           // Sample Datagram 
			SampleDatagram *sd = (SampleDatagram *)malloc(sizeof(text));
			memcpy(sd,(void *)text,len);
			if (sd->DgHeader.DatagramType == 811024722){   //determine the TYPE is the value "811024722"   
				saveSampleDatagram(output, sd);
			}
			else{
				printf("End!\n");
				//break;
			}	
			free(sd);			
		}
		else{		//NMEA Aatagram
			
			TextDatagram *td = (TextDatagram *)malloc(sizeof(text));
			memcpy(td,(void *)text, len);
			//printf("%s\n",(unsigned char*)td->Text);
			saveTextDatagram(output, td);
			free(td);
		}	
		
		fread(&len2,sizeof(long),1,file); 
		//printf("%d\n", len2);
	
	}	
	
    fclose(file);
    fclose(output);
    free(cd);

    	
	return 0;
}

