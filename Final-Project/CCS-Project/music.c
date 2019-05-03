/*
 * music.c
 *
 *  Created on: Apr 24, 2019
 *      Author: Conor Albinger, Adis Osmankic
 */
#include "open_interface.h"
#include "movement.h"
#include "lcd.h"
#include "uart.h"

void play_song(oi_t *sensor, int song_index){


    if(song_index==0){
        //still alive
        //http://www.inspiredacoustics.com/en/MIDI_note_numbers_and_center_frequencies
        //https://pianoletternotes.blogspot.com/2018/04/still-alive-portal.html#quickg


        char notes[]=   {79,78,76,76,78,10,79,78,76,76,78,74,76,10,76};
        char duration[]={13,13,13,13,13,96,13,13,13,20,25,20,25,50,25};
        oi_loadSong(1,15,notes,duration);
        oi_play_song(1);
    }
    else if(song_index==1){
        //sandstorm
                char d=62;
                char b=59;
                char e=64;
                char a=57;
        char notes[]=   {59,59,59,59,59,59,59,59,59,59,59,59,64,64,64,64,64,64,64,62,62,62,62,d,d,d,a,a,b,b,b,b,b,b,b,b,b,b,b,b,e,e,b,b,b,b};
        char duration[]={8,8,8,8,16,8,8,8,8,8,8,16,8,8,8,8,8,8,16,8,8,8,8,8,8,16,8,8,8,8,8,8,16,8,8,8,8,8,8,16,8,8,8,8,8,8};

        oi_loadSong(1, 46,notes,duration);
        oi_play_song(1);
    }

    else if(song_index==2){
        //nyan cat
        char notes[]=   {63,64,66,71,63,64,66,71,73,75,73,70,71,66,63,64,66,71,73,70};
        char duration[]={8 ,8 ,16,16,8 ,8 ,8 ,8 ,8 ,8 ,8 ,8,16,16 ,8 ,8 ,16, 16, 8,8};
        oi_loadSong(1, 20,notes,duration);
        oi_play_song(1);
    }

    else if(song_index==3){
        //all star
        char notes[]=   {66,73,70,70,68,66,66,71,70,70,68,68,66,66,73,70,70,68,68,66,66,63};
        char duration[]={32,16,16,32,16,16,16,32,16,16,16,16,32,16,16,16,16,16,16,16,16,16};
        oi_loadSong(1, 22,notes,duration);
        oi_play_song(1);

    }

    else if(song_index==4){
        //mii channel
        char notes[]={66, 69, 73, 69, 66, 62, 62, 62, 66, 69, 73, 69};
        char duration[]={4, 16, 8, 16, 16, 8, 8, 8, 36, 8, 8, 16};
        oi_loadSong(1, 12,notes,duration);
        oi_play_song(1);

    }
    else if(song_index==5){
        //chicken dance
        char c6=84;
        char d5=74;
        char e5=76;
        char f5=77;
        char g5=79;
        char g4 =67;
        char f4= 65;
        char d4=62;
        char a5=81;
        char b5=83;
        char b4=71;
        char y = 24;
        char z = 16;
        char rest = 10;
        char notes[]={g5,g5,a5,a5,e5,e5,g5,rest,g5,g5,a5,a5,e5,e5,g5,rest,g5,g5,a5,a5,c6,c6,b5,a5,g5,f5,d5,f5,f5,g5,g5,d5,d5,f5,g5,g5,a5,a5,b5,b5,a5,rest};
        char duration[]={z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z,y,y,y,y,y,z,z,z,z,z,z,z,z,z,z,z,z,z,z,z};
        oi_loadSong(1,42,notes,duration);
        oi_play_song(1);
    }

}
