#!/bin/bash
fileDir=${PWD}/media 
#ffmpeg -i /root/iptest.mp4  -i /root/huaqian.mp4 -i /root/iptest.mp4 -filter_complex "[0:v]pad=iw*3:ih*1[a];[a][1:v]overlay=w[b];[b][2:v]overlay=w*2" /root/ninebar.mp4; 
#ffmpeg  -i $fileDir/ipres.mp4  -i $fileDir/ip2.mp4  -i $fileDir/ip2.mp4 -filter_complex "nullsrc=size=1080x960[base];[0:v]scale=540x960[a];[1:v]scale=540x480[b];[2:v]scale=540x480[c];[base][a]overlay=shortest=1[tmp1];[tmp1][b]overlay=x=540[tmp2];[tmp2][c]overlay=x=540:y=480"  -c:v libx264 /root/ninebar.mp4 -y 
#ffmpeg -re -i $fileDir/iptest.mp4 -re -i $fileDir/huaqian.mp4 -re -i $fileDir/iptest.mp4 -filter_complex "nullsrc=size=1080x960[base];[0:v]scale=540x960[a];[1:v]scale=540x480[b];[2:v]scale=540x480[c];[base][a]overlay=shortest=1[tmp1];[tmp1][b]overlay=shortest=1:x=540[tmp2];[tmp2][c]overlay=shortest=1:x=540:y=480" -c:v libx264 /root/ninebar.mp4; 
ffmpeg  -i $fileDir/ipres.mp4  -i $fileDir/ip2.mp4  -i $fileDir/white.mp4 -filter_complex "nullsrc=size=900x480[base];[0:v]scale=540x480[a];[1:v]scale=360x240[b];[2:v]scale=360x240[c];[base][a]overlay=shortest=1[tmp1];[tmp1][b]overlay=x=540[tmp2];[tmp2][c]overlay=x=540:y=240"  -c:v libx264 /root/ninebar.mp4 -y 
