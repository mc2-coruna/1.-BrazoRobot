#include "ofApp.h"


/****  PARA HACER  ****
________________________
 
Leer mensaje desde brazorobot
Pedir poseRead
Leer estado y fuerza de los servos
Retardar la actualizaci—n de imagen desde la pulsaci—n

A–adir imagen de brazo de perfil
A–adir iconos de control y escenograf’a

________________________
****               ****/


//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(30);
    
    
    /**********************************************************************
    *           SetUp some instances and global variables                *
    **********************************************************************/

     // Inicializa tipos de letra
    mono09.load("mono.ttf",  9);
    mono12.load("mono.ttf", 12);
    mono14.load("mono.ttf", 14);
    mono16.load("mono.ttf", 16);
    mono18.load("mono.ttf", 18);
    mono24.load("mono.ttf", 24);
    mono36.load("mono.ttf", 36);
    mono48.load("mono.ttf", 48);
    
    // Determina la escala de las im‡genes
    escalaH = float(ofGetWindowWidth())/1920;
    escalaV = float(ofGetWindowHeight())/1080;
    escala  = int(50*min(escalaH,escalaV))/50.0
    
    
    ;
    printf("Tamano ventana %i x %i; escala = %f \n\n",
           ofGetWindowWidth(),ofGetWindowHeight(),escala);
    
    ofSetColor(255,255,255);  //À?
    
    if (missionImage.load("Mission1.jpg"))
        printf ("imagen cargada de %4i x %4i \n", (int)missionImage.getWidth(), (int)missionImage.getHeight());
    else         printf ("no carga ni de cona \n");

    

    
    /**********************************************************************
     *                    SetUp the video devices                         *
     **********************************************************************/
    // List videoDevices
    printf("\n------------\n      ** Video Devices \n\n");
    vector<ofVideoDevice> devices = brazoBotGrabber.listDevices();
    for(int i = 0; i < devices.size(); i++){
        if(devices[i].bAvailable){                       // log the device
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
            
            if (devices[i].deviceName.find("LifeCam") != string::npos) {
                brazoBotCamID = i;}                      // Check if its brazoBbotCam
            
            if (devices[i].deviceName.find("Logitech") != string::npos) {
                personalCamID = i;}                      // Check if its personalCam
        }
        else{
            ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";            //log the device and note it as unavailable
        }
    }

        /**********************************************************************
         *       This is just for test purpose with the FaceTime cam           *
         **********************************************************************/
    if (devices.size()>0){
        if (brazoBotCamID<0) brazoBotCamID = 0;
        if (personalCamID<0) personalCamID = 0;
    }

    brazoBotCamID = 0;

    
    //Inicializa la c‡mara del brazoBot
    if ((devices.size() > 0) and (brazoBotCamID >=0)){
        brazoBotGrabber.setDeviceID(brazoBotCamID);
        brazoBotGrabber.initGrabber(1280,800);
        brazoBotGrabberInit = true;
        printf("\nInicializada la camara del brazoBot con ID %i y resolucion %i by %i \n", brazoBotCamID,
            (int)brazoBotGrabber.getWidth(), (int)brazoBotGrabber.getHeight());
    }
    else printf("  ******  Fallo al inicializar la camara principal del brazoBot  ******\n\n");
    nextRefreshTime = ofGetElapsedTimeMillis();

/*
    //Inicializa la c‡mara personal
    if ((devices.size() > 0) and (personalCamID >=0)){
        personalCamGrabber.setDeviceID(personalCamID);
        personalCamGrabber.initGrabber(640,480);
        personalCamGrabberInit = true;
        printf("\nInicializada la camara personal con ID %i y resolucion %i by %i \n", personalCamID,
            (int)personalCamGrabber.getWidth(), (int)personalCamGrabber.getHeight());
    }
    else printf("\n  ******  Fallo al inicializar la camara personal del brazoBot  ******\n");

 
    
    /**********************************************************************
    *                SetUp the BrazoRobot Serial Port                    *
    **********************************************************************/
    vector<ofSerialDeviceInfo> serialDevices = brazoBotSerial.getDeviceList();
    printf("\n------------\n      ** Serial Devices \n");
    for(int i = 0; i < serialDevices.size(); i++){
        ofLogNotice() << serialDevices[i].getDeviceID() << ": " << serialDevices[i].getDeviceName() << "; " << serialDevices[i].getDevicePath();

        if (serialDevices[i].getDeviceName().find("12KDFD0E") != string::npos)
            brazoBotSerialID = i;
    }
    printf("------------\n");

    if (brazoBotSerialID >=0){
        brazoBotSerial.setup(brazoBotSerialID, 38400);
        if( brazoBotSerial.isInitialized() ){
            brazoBotSerialInited = true;
            printf("brazoRobotSerial is setup! \n\n");
        }
        else printf("  ******  Fallo al inicializar la comunicacion con el brazoBot  ******\n\n");
    }

    keyBuffer.indexMax = keyBufferSize;
    if (keyBuffer.indexMax > 20) keyBuffer.indexMax = 20;
    
    /**********************************************************************
     *                    SetUp xxxxxxxxxxxxxxxxxx                         *
     **********************************************************************/
    // Sets the brazoBot limits and variables
    
    base.angle     =   0;
    shoulder.angle = 179;
    elbow.angle    =   1;
    wrist.angle    = 180;
    
    
    base.angle_min     = -85;
    base.angle_Max     =  85;
    
    shoulder.angle_min =  35;
    shoulder.angle_Max = 135;
    shoulder.ro_joint  =   0;
    shoulder.h_joint   = 110;
    
    elbow.angle_min    =  15;
    elbow.angle_Max    = 160;
    
    wrist.angle_min    =  60;
    wrist.angle_Max    = 160;
    wrist.ro_min       =  75;
    wrist.ro_Max       = 280;
    wrist.h_min        = 110;
    wrist.h_Max        = 310;
    wrist.x_min        =-175;
    wrist.x_Max        = 175;
    wrist.y_min        =   0;
    wrist.y_Max        = 215;
    
    hand.angle_min     =-150;
    hand.angle_Max     = 150;
    hand.ro_min        =  75;
    hand.ro_Max        = 355;
    hand.h_min         =  15;
    
    
    lastOrderSeconds = ofGetElapsedTimef();
}


//--------------------------------------------------------------
void ofApp::update(){
    
    unsigned long actualSeconds = ofGetElapsedTimef();
    unsigned long actualMilliSeconds = ofGetElapsedTimeMillis();
 
    
    /**********************************************************************
    *              Inits the brazoBot 10 secs after Start                 *
    **********************************************************************/
    // Inicializa el robot 10 segundos despuŽs de iniciar el programa
    if ((!brazoBotInited) and (actualSeconds>10)){
            brazoBotInitBH();           // Inicializa el barzoBot en modo BackHoe
            brazoBotInited=true;
    }

    
    /**********************************************************************
    *          Check if there is new images from the grabbers             *
    **********************************************************************/
    // Updates the brazoBotCam
    
    /*
    if  (brazoBotGrabberInit and
         (frameCount%brazoBotGrabberFrameRefres == 0) and
         (actualMilliSeconds > nextRefreshTime))
     */
    
    if  (brazoBotGrabberInit and
        (frameCount%brazoBotGrabberFrameRefres == 0))
            brazoBotGrabber.update();  // call this once per update
    
    // Updates the personalCam
    if (personalCamGrabberInit) personalCamGrabber.update();
    
    
    /**********************************************************************
    *               Reads from the BrazoBot Serial port                   *
    **********************************************************************/
    char caracter;
    
    if (brazoBotSerial.isInitialized()){
        if (!brazoBotSerialInited) printf ("serialPort Ok\n");
        brazoBotSerialInited = true;

        while (brazoBotSerial.available() > 0) {
            caracter = brazoBotSerial.readByte();
            printf("%c",caracter);
        }
    }
    else{
        if (brazoBotSerialInited)
            printf ("Lost serial Port connection \n");
        brazoBotSerialInited = false;
    }
    

    
    /**********************************************************************
     *           Manages the key buffer                                    *
     **********************************************************************/
    static unsigned long nextKeyMillis = 0;
    string command = "";
    float parameter = 0;
    float delta_phi=0, delta_ro=0, delta_h=0, delta_w=0, delta_wr=0;
    
    if (keyBuffer.index > 0){     // hay teclas en el buffer
        if ((actualMilliSeconds > nextKeyMillis) and
            (keyBuffer.timeStamp[0]+1000 < actualMilliSeconds))
        {
            command   = keyBuffer.command[0];
            parameter = keyBuffer.parameter[0];
            for (int i=0; i<keyBuffer.indexMax-1; i++){
                keyBuffer.timeStamp[i] = keyBuffer.timeStamp[i+1];
                keyBuffer.command[i]   = keyBuffer.command[i+1];
                keyBuffer.parameter[i] = keyBuffer.parameter[i+1];
            }
            keyBuffer.index --;
            nextKeyMillis = actualMilliSeconds + 2000;
     
            
            if      ((command=="FWD") or (command=="BAK")) delta_ro  = parameter;
            else if ((command=="RGT") or (command=="LFT")) delta_phi = parameter;
            else if ((command=="UP ") or (command=="DWN")) delta_h   = parameter;
            else if ((command=="INS") or (command=="OUT")) delta_w   = parameter;
            else if ((command=="WRL") or (command=="WRR")) delta_wr  = parameter;

            
            brazoBotDelta_phi_ro_h (delta_phi, delta_ro, delta_h, delta_w, delta_wr);
            
            printf ("%.3f >> enviado dphi %4.2f, dro %4.2f, dh %4.2f, dw %4.2f", (float)actualMilliSeconds/1000., delta_phi, delta_ro, delta_h, delta_w);
        }
    }
    

    
    
    
    
    /**********************************************************************
    *           Performs a little movement each 10 minutes                *
    **********************************************************************/
    if (actualSeconds > lastOrderSeconds+600){
        lastOrderSeconds = actualSeconds;
        brazoBotDelta_phi_ro_h (ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1), ofRandom(-1,1));
        printf("enviado comando de mantenimiento \n");
    }

    
    if (brazoBotInited){
        botStrcColor.r = 0;
        botStrcColor.g = 255;
        botStrcColor.b = 0;
    }
    else{
        if (frameCount%50 > 25){
            botStrcColor.r = 250;
            botStrcColor.g = 250;
            botStrcColor.b = 20;
        }
        else {
            botStrcColor.r = 150;
            botStrcColor.g = 50;
            botStrcColor.b = 5;
        }
    }
    
    botShoulderColor.r = 255;
    botShoulderColor.g = 0;
    botShoulderColor.b = 0;
    
    botElbowColor.r = 0;
    botElbowColor.g = 255;
    botElbowColor.b = 0;
    
    botWristColor.r = 0;
    botWristColor.g = 0;
    botWristColor.b = 255;
    
}

//--------------------------------------------------------------
void ofApp::draw(){
   
    
    timers();
    ofSetBackgroundColor(0,0,0);
    
    ofPushMatrix();
    {
        
        ofScale (escala, escala);
        
        ofSetColor (255);
        ofNoFill();
        ofDrawRectangle(1,1,1918,1078);

        drawBrazoBotTopView(1400,20,0,0);
 
        drawBrazoBotCam(20, 20, 1366, 768, 0);
        drawPersonalCam(1040, 820, 320, 240);

    
        drawInstructions(1400, 840, 760, 240);
        
        drawSurface(1400, 440);
    }
    ofPopMatrix();
}

//--------------------------------------------------------------
void ofApp::drawSurface(float x, float y){
    

    static int alpha = 0;
    
    ofPushMatrix();
    {
        ofTranslate(x,y);
        //ofScale(1.6,1.6);
    
        ofSetColor (0,255,200);
        missionImage.draw(0, 0, 500, 300);
        ofSetColor (0,255,0);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawRectangle(0,0, 500, 300);
 
        ofTranslate (250, 0);
    
        // Draws robot position
        ofSetColor (255);
        ofNoFill();
        ofDrawCircle(0,0,10);
        

    
        // Draws the goal position
        ofSetColor(255,0,0,alpha);
        ofFill();
        ofDrawCircle(47,116,7);
        alpha -= 5;
        if (alpha < 0) alpha = 255;
        
        
        // draws the wrist position
        ofSetColor (255);
        ofSetLineWidth(0.5);
        
        float sinBase = sin(ofDegToRad(base.angle));
        float cosBase = cos(ofDegToRad(base.angle));
        for (int i = 0; i<10; i++){
            ofDrawLine(30*i*sinBase, 30*i*cosBase, (30*i+10)*sinBase, (30*i+10)*cosBase);
        }
            
        
        
        
        ofPath angleArc;
        
        angleArc.setFilled(false);
        angleArc.setStrokeWidth(2);
        
        // Draws the BASE angle
        angleArc.setStrokeColor(ofColor(0,255,0));
     //   angleArc.moveTo (0,50);
        if (base.angle > 0.1)
            angleArc.arcNegative (0,0,50, 50, 90, 90 - base.angle);
        else if (base.angle < -0.1)
            angleArc.arc (0,0,50, 50, 90, 90 - base.angle);
        else
            angleArc.lineTo (0,50);
        angleArc.lineTo (0,0);
        angleArc.draw();
        angleArc.clear();
        
        
        base.angle_draw = base.angle;
        base.ro_label = 40 * sin(ofDegToRad(base.angle/2));
        base.h_label  = 40 * cos(ofDegToRad(base.angle/2));

        sprintf(base.angle_label,"%5.1f",base.angle_draw);
        drawLabel (base.ro_label,         base.h_label,
                   60,       60,
                   base.angle_label,       base.labelColor,
                   base.textColor,         mono14);

        
        
        
        
        
        
        
        
        
        ofTranslate(wrist.x_joint*1.30, wrist.y_joint*1.30);
    
        ofSetColor (255);
        ofSetLineWidth(1);
        ofDrawLine(-20, 0, 20, 0);
        ofDrawLine(0, -20, 0, 20);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawCircle(0,0,7);
        ofFill();
        ofSetColor (255,100);
        ofSetLineWidth(1);
        ofDrawCircle(0,0,7);
    
        sprintf (wrist.pos_label,"(%4.1f, %4.1f)",wrist.x_joint, wrist.y_joint);
        if (wrist.x_joint > 0)
            drawLabel (0,0, -20, 20, wrist.pos_label, ofColor (255), ofColor (255), mono14);
        else drawLabel (0,0, 20, 20, wrist.pos_label, ofColor (255), ofColor (255), mono14);
    }
    ofPopMatrix();
}


//--------------------------------------------------------------
void ofApp::timers(){
 
    frameCount ++;
    if (frameCount%25 < 20){
        alertColor1 = ofColor (255,255,0);
        alertColor2 = ofColor (255,0,0);
    }
    else{
        alertColor1 = ofColor (0);
        alertColor2 = ofColor (0);
    }
}

//--------------------------------------------------------------
void ofApp::drawBrazoBotCam(int x, int y, int width, int height_, float wr){
    
    char texto[256];
    
    int height = width * .75;

    ofPushMatrix();
    {
    
    ofTranslate(x, y, 0);
    
    ofSetColor(0,255,0);
    ofNoFill();
    ofSetLineWidth(2);
    ofDrawRectangle(-2, -2, width+4, height+4);
    
    ofSetColor (255,255,255);
        if (brazoBotGrabberInit){
            ofPushMatrix();{
                ofTranslate(width, height);
                ofRotate (180);
                brazoBotGrabber.draw(0, 0, width, height);
            }
            ofPopMatrix();
        }
    else{
        ofSetColor (255,0,0);
        mono48.drawString("sin imagen",width/2-100, height/2+24);}
    
        ofSetColor (0,150);
        ofFill();
        ofDrawRectangle(5,5,275,800);
        
        ofSetColor (100,255,100,75);
        ofDrawRectangle(5,5,275,800);
        
    
    ofSetColor (100,255,100);
    sprintf(texto,"*****  WRIST  ******\n>base angle:    %4.1f\n>shoulder ang:  %4.1f\n>elbow angle:   %4.1f \n>wrist angle:   %4.1f\n>w rotat angle: %4.1f",
            base.angle_draw ,shoulder.angle_draw,
            elbow.angle_draw, wrist.angle_draw, wr);
    mono16.drawString(texto, 10,50);

        
        ofSetColor (0,150);
        ofFill();
        ofDrawRectangle(5,970,1200,50);
        ofSetColor (100,255,100,75);
        ofDrawRectangle(5,970,1200,50);

        ofSetColor (100,255,100);
        int milis   = ofGetElapsedTimeMillis() % 1000;
    int seconds = ofGetElapsedTimeMillis() / 1000;
    unsigned long frames  = ofGetFrameNum();
    sprintf(texto,"time %6i:%3i   frame %7lu",seconds ,milis, frames);
    mono24.drawString(texto, width/2-380,height-12);
    
    
    /***********************************************************
    
    sprintf(texto,"wrist \n   >base angle: %4.1f\n   >shoulder angle:  %4.1f\n   >elbow angle: %4.1f \n   >wrist angle: %4.1f\n   >wristo rotation angle: %4.1f\n   >grip: %4.1f \n\nhand %4.1f, %4.1f, %4.1f ;(%4.1f, %4.1f, %4.1f) ",
            brazoBot_phi,brazoBot_wrist_ro, brazoBot_wrist_h,
            brazoBot_wrist_x, brazoBot_wrist_y, brazoBot_wrist_z,
            brazoBot_phi,brazoBot_hand_ro, brazoBot_hand_h,
            brazoBot_hand_x, brazoBot_hand_y, brazoBot_hand_z);
    
     ***********************************************************/
    
    
    }
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::drawPersonalCam(int x, int y, int width, int h_){
    
    int height = width * 0.75;
    
    ofPushMatrix();
    {
        ofTranslate(x, y, 0);
    
        ofSetColor(0,255,0);
        ofNoFill();
        ofSetLineWidth(2);
        ofDrawRectangle(-2, -2, width+4, height+4);
        ofSetColor (255,255,255);
        if (personalCamGrabberInit)
            personalCamGrabber.draw(0, 0, width, height);
        else{
            ofSetColor (255,0,0);
            mono24.drawString("sin imagen",width/2-50, height/2+12);}
    }
    ofPopMatrix();
    
}

//--------------------------------------------------------------
void ofApp::drawInstructions(int x, int y, int w, int h){
    
    
    char texto[256];
    string texto2 = "";
    
    for (int i=0; i<keyBuffer.index; i++){
        sprintf(texto,">>> TS  %8lu  movimiento [%2i]: %s   %5.2f uds", keyBuffer.timeStamp[i], i, keyBuffer.command[i].c_str(), keyBuffer.parameter[i]);
        
        texto2.append(ofToString(texto));
        texto2.append("\n");
    }
    
    ofPushMatrix();{
        ofTranslate(x, y, 0);
        ofFill();

        ofSetColor (0,55,0);
        ofDrawRectangle(0, 0, w, h);
    
        ofSetColor (0,255,0);
        mono16.drawString("> a > -X; d > +X    \nz > -Y; w > +Y\n> 1 > -Z; 2 > +Z    \n> 4 > -W; 5 > +W    \n> 7 > -G; 8 > +G",20,20);
    
        //sprintf(texto,">>> Incremento= %i",int(brazoBotDelta));
        
        
        mono16.drawString(texto, 20,200);
        mono12.drawString(texto2, 20,-60);
    
        
    }
    ofPopMatrix();
    
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::drawBrazoBotTopView (int x, int y, int width, int height){

    float humerusLenght = brazoBotArmLenght;
    float radiusLenght  = brazoBotArmLenght;
    float handLenght    = brazoBotHandLenght;
    float random_movement;
    ofPath angleArc;


    /**********************************************************************
    *             Calculate the angles with some blur movement            *
    **********************************************************************/
    random_movement = .0;
    
    if (frameCount%brazoBotGrabberFrameRefres == 0){
        shoulder.angle_draw = shoulder.angle +
                ofRandom(-random_movement, random_movement);
        elbow.angle_draw    = elbow.angle +
                ofRandom(-random_movement, random_movement);
        wrist.angle_draw    = wrist.angle +
                ofRandom(-random_movement, random_movement);
    }
    

    /**********************************************************************
     *                   Calculate some data of the joints                 *
     **********************************************************************/
    shoulder.angle_ant  = 0;
    shoulder.angle_post = shoulder.angle_draw;

    shoulder.ro_draw  = shoulder.ro_joint;
    shoulder.h_draw   = shoulder.h_joint;
    shoulder.ro_label = shoulder.ro_draw + 40 * cos(ofDegToRad(shoulder.angle/2));
    shoulder.h_label  = shoulder.h_draw  + 40 * sin(ofDegToRad(shoulder.angle/2));
    
    
    
    elbow.angle_ant  = 180 + shoulder.angle_post;
    elbow.angle_post = elbow.angle_ant + elbow.angle_draw;
    
    elbow.ro_draw = shoulder.ro_draw + humerusLenght * cos(ofDegToRad(shoulder.angle_post));
    elbow.h_draw  = shoulder.h_draw  + humerusLenght * sin(ofDegToRad(shoulder.angle_post));
    
    elbow.ro_label = elbow.ro_draw +
            40 * cos(ofDegToRad((elbow.angle_ant+elbow.angle_post)/2));
    elbow.h_label  = elbow.h_draw  +
            40 * sin(ofDegToRad((elbow.angle_ant+elbow.angle_post)/2));
    
    
    
    wrist.angle_ant  = 180 + elbow.angle_post;
    wrist.angle_post = wrist.angle_ant + wrist.angle_draw;

    wrist.ro_draw = elbow.ro_draw + radiusLenght * cos(ofDegToRad(elbow.angle_post));
    wrist.h_draw  = elbow.h_draw  + radiusLenght * sin(ofDegToRad(elbow.angle_post));
    
    wrist.ro_label = wrist.ro_draw +
            40 * cos(ofDegToRad((wrist.angle_ant+wrist.angle_post)/2));
    wrist.h_label  = wrist.h_draw  +
            40 * sin(ofDegToRad((wrist.angle_ant+wrist.angle_post)/2));
 
    
    
    hand.ro_draw = wrist.ro_draw + handLenght * cos(ofDegToRad(wrist.angle_post));
    hand.h_draw  = wrist.h_draw  + handLenght * sin(ofDegToRad(wrist.angle_post));

    
    
    /**********************************************************************
     *                       Moves and scales the image                    *
     **********************************************************************/
    
    
    ofPushMatrix();
    {
        ofTranslate(x,y);
        ofTranslate(75,400);
        
        ofNoFill();
        ofSetColor (ofColor (0,255,0));
        ofSetLineWidth(2);
        ofDrawRectangle(-75,-400,500,400);   // reference frame
    
        if (brazoBotSerialInited){
            ofSetColor (0,255,0);
            mono24.drawString("connected mode", 0,-360);
        }
        else{
            ofSetColor (255,0,0);
            mono24.drawString("SIMULATION MODE", 0,-360);
        }
        
        
        
        angleArc.moveTo (-70,  -4);
        angleArc.lineTo ( 70,  -4);

        angleArc.lineTo ( 50,-110);
        angleArc.lineTo (-50,-110);
        angleArc.lineTo (-70,  -4);
        
        angleArc.setFillColor (ofColor(0,75,75));
        angleArc.setFilled(true);
        angleArc.setStrokeWidth(0);
        angleArc.draw();
        
        angleArc.setStrokeColor(ofColor(0,175,175));
        angleArc.setFilled(false);
        angleArc.setStrokeWidth(4);
        angleArc.draw();
        
        angleArc.clear();
        
        
        /**********************************************************************
         *             Draws the angles (behind the arm sections)              *
         **********************************************************************/
        angleArc.setFilled(false);
        angleArc.setStrokeWidth(2);
        
        // Draws the SHOULDER angle
        angleArc.setStrokeColor(ofColor(0,255,0));
        //angleArc.moveTo (shoulder.x_draw+50, shoulder.y_draw);
        angleArc.moveTo (shoulder.ro_draw, -shoulder.h_draw);
        angleArc.arcNegative (shoulder.ro_draw, -shoulder.h_draw,50, 50,
                    -shoulder.angle_ant, -shoulder.angle_post);
        angleArc.draw();
        angleArc.clear();
        
        // Draws the ELBOW angle
        angleArc.setStrokeColor(ofColor(0,255,0));
        angleArc.arcNegative (elbow.ro_draw, -elbow.h_draw, 50, 50,
                    -elbow.angle_ant, -elbow.angle_post);
        angleArc.draw();
        angleArc.clear();

        // Draws the WRIST angle
        angleArc.clear();
        angleArc.arcNegative (wrist.ro_draw, -wrist.h_draw, 50, 50,
                    -wrist.angle_ant, -wrist.angle_post);
        angleArc.draw();
        angleArc.clear();

        
        /**********************************************************************
         *                   Draws the robot arm sections                      *
         **********************************************************************/
        // Draws the HUMERUS section
        brazoBotDrawSegment (shoulder.ro_draw, -shoulder.h_draw,humerusLenght,15,
            shoulder.angle_post,botStrcColor, botShoulderColor);
        
        // Draws the RADIUS section
            botStrcColor.r = 255;
            botStrcColor.g = 0;
            botStrcColor.b = 0;
        brazoBotDrawSegment (elbow.ro_draw, -elbow.h_draw,
                    radiusLenght,15, elbow.angle_post,botStrcColor, botElbowColor);
        
        // Draws the HAND section
            botStrcColor.r = 0;
            botStrcColor.g = 0;
            botStrcColor.b = 255;
        brazoBotDrawSegment (wrist.ro_draw, -wrist.h_draw,
                    handLenght-15,15, wrist.angle_post,botStrcColor, botWristColor);
        
        // Draws the camera indicator
        ofPushMatrix();{
            ofTranslate (wrist.ro_draw, -wrist.h_draw);
            ofRotate (-wrist.angle_post, 0.0, 0.0,1.0);
            ofTranslate (handLenght, 0);
            ofFill();
            ofSetColor (ofColor(255,0,0));
            ofDrawRectangle (-75,-12,75,24);
            
            ofSetColor (255);
            mono14.drawString ("CAMERA", -70, 8);
        }
        ofPopMatrix();

        /**********************************************************************
         *             Draws the joint labels                                  *
         **********************************************************************/
        // Draws the Elbow position
        sprintf (elbow.pos_label,"(%4.1f, %4.1f)",elbow.ro_draw, elbow.h_draw);
        drawLabel (elbow.ro_draw, -elbow.h_draw, elbow.ro_draw-30, -elbow.h_draw-30,
                   elbow.pos_label, ofColor(0,0,255), ofColor(255,0,0), mono14);
        
        // Draws the Wrist position
        sprintf (wrist.pos_label,"(%4.1f, %4.1f)", wrist.ro_draw, wrist.h_draw);
        drawLabel (wrist.ro_draw, -wrist.h_draw, wrist.ro_draw+20, -wrist.h_draw-20,
                   wrist.pos_label, ofColor(0,0,255), ofColor(255,0,0), mono14);

        // Draws the Hand position
        sprintf (hand.pos_label,"(%4.1f, %4.1f)",hand.ro_draw, hand.h_draw);
        drawLabel (hand.ro_draw, -hand.h_draw, hand.ro_draw+25, -hand.h_draw+25,
                   hand.pos_label, ofColor(0,0,255), ofColor(255,0,0), mono14);
     
        /**********************************************************************
         *             Draws the angle labels                                  *
         **********************************************************************/
        // Draws the SHOULDER_ANGLE label
        sprintf(shoulder.angle_label,"%5.1f",shoulder.angle_draw);
        drawLabel (shoulder.ro_label,      -shoulder.h_label,
                   shoulder.ro_draw+60,    -shoulder.h_draw-20,
                   shoulder.angle_label,   shoulder.labelColor,
                   shoulder.textColor,     mono14);

        // Draws the ELBOW_ANGLE label
        sprintf(elbow.angle_label,"%5.1f", elbow.angle_draw);
        drawLabel (elbow.ro_label,       -elbow.h_label,
                   elbow.ro_label-40,    -elbow.h_label-40,
                   elbow.angle_label,    elbow.labelColor,
                   elbow.textColor,      mono14);
       
        // Draws the WRIST_ANGLE label
        sprintf(wrist.angle_label,"%5.1f", wrist.angle_draw);
        drawLabel (wrist.ro_label,       -wrist.h_label,
                   wrist.ro_label-30,    -wrist.h_label-35,
                   wrist.angle_label,    wrist.labelColor,
                   wrist.textColor,      mono14);
        
        
    }
    ofPopMatrix();
    

}

//--------------------------------------------------------------
void ofApp::brazoBotDrawSegment (float X0, float Y0, float distanciaEjes, float ancho,  float angulo, ofColor structColor, ofColor jointColor){
    
    ofColor colorFondo = ofGetBackgroundColor();
    
    // desplaza el origen de coordenadas a X0, Y0
    ofPushMatrix();
    ofTranslate(X0,Y0);
    ofRotate(-angulo,0.0,0.0,1.0);
    
    // dibuja la parte central del brazo
    ofFill();
    ofSetColor(colorFondo);
    ofDrawRectRounded (-(ancho/2),-(ancho/2),distanciaEjes+ancho,ancho,ancho/2);
    
    ofNoFill();
    ofSetColor(structColor,75);
    ofSetLineWidth(11);
    ofDrawRectRounded (-(ancho/2),-(ancho/2),distanciaEjes+ancho,ancho,ancho/2);
    
    ofSetLineWidth(0.5);
    ofSetColor(structColor);
    ofDrawRectRounded (-(ancho/2),-(ancho/2),distanciaEjes+ancho,ancho,ancho/2);
    
    ofFill();
    ofSetColor(jointColor);
    ofDrawCircle(0, 0, ancho/2);
    
    ofSetColor(colorFondo);
    ofDrawLine(0,-4,0,4);
    ofDrawLine(-4,0,4,0);
    ofDrawLine(distanciaEjes,-4,distanciaEjes,4);
    ofDrawLine(distanciaEjes-4,0,distanciaEjes+4,0);
    ofPopMatrix();
    
    
    
    
    
    
    
}

//--------------------------------------------------------------
void ofApp::drawLabel (float xAnchor, float yAnchor, float xLabel, float yLabel, char labelText[256], ofColor labelColor, ofColor textColor, ofTrueTypeFont textFont)
{
    
    int textWidth = textFont.stringWidth(labelText);
    int xLineEnd;
    int xText;
    ofPath labelPath;

    if (xAnchor < xLabel){
        xLineEnd = xLabel + textWidth/2 ;
        xText    = xLabel +5;
    }
    else{
        xLineEnd = xLabel - textWidth/2 ;
        xText    = xLabel - textWidth -5;
    }
    
    
    labelPath.setStrokeWidth(1);
    labelPath.setStrokeColor(labelColor);
    labelPath.setFilled(false);
    
    labelPath.moveTo (xAnchor, yAnchor);
    labelPath.lineTo (xLabel,  yLabel);
    labelPath.lineTo (xLineEnd, yLabel);
    labelPath.draw();
    
    ofSetColor(textColor);
    textFont.drawString(labelText, xText, yLabel -5);
    

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    float delta_phi = 0;
    float delta_ro  = 0;
    float delta_h   = 0;
    float delta_w   = 0;
    float delta_wr = 0;
    string command  = "";
    float parameter = 0.0;

    
    if      (key == 'V') brazoBotDelta =2.0;
    else if (key == 'B') brazoBotDelta =5.0;
    
    
    // Front and back movement
    if      (key == 'a') {
        command = "FWD";
        parameter =  brazoBotDelta;       // ro
    }
    else if (key == 'b') {
        command = "BAK";
        parameter = -brazoBotDelta;
    }
    

    // Left and right movement
    else if (key == 'c'){
        command = "RGT";
        parameter =  brazoBotDelta * 60/wrist.ro_joint;  //phi
    }
    else if (key == 'd'){
        command = "LFT";
        parameter = -brazoBotDelta * 60/wrist.ro_joint;
    }
    
    
    // up and down movement
    else if (key == 'e'){
        command = "UP ";
        parameter =  brazoBotDelta;    // h
    }
    else if (key == 'f'){
        command = "DWN";
        parameter = -brazoBotDelta;
    }

    
    // Wrist insie/ouside movement
    else if (key == 'g')
    {
        command = "INS";
        parameter =  brazoBotDelta;  // w movement
    }
    else if (key == 'h'){
        command = "OUT";
        parameter = -brazoBotDelta;
    }

    
    // Wrist rotation movement
    else if (key == 'i'){
        command = "WRL";
        parameter =  brazoBotDelta * 60/wrist.ro_joint;  //WR
    }
    else if (key == 'j'){
        command = "WRR";
        parameter =  brazoBotDelta * 60/wrist.ro_joint;
    }

    if (command != ""){
        if (keyBuffer.index < keyBuffer.indexMax){
            keyBuffer.timeStamp[keyBuffer.index] = ofGetElapsedTimeMillis();
            keyBuffer.command  [keyBuffer.index]  = command;
            keyBuffer.parameter[keyBuffer.index] = parameter;
            keyBuffer.index ++;
        }
 
    }
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}


//--------------------------------------------------------------
void ofApp::brazoBotDelta_phi_ro_h (float d_phi, float d_ro, float d_h, float d_w, float d_wr){
    
    
    // New Wrist coordinates
    float phiNext      = base.angle      + d_phi;        // confirmar brazoBot_xMuneca
    float wristNext_ro = wrist.ro_joint  + d_ro;         // confirmar brazoBot_xMuneca
    float wristNext_h  = wrist.h_joint   + d_h;          // confirmar brazoBot_xMuneca
    float wristNext    = wrist.angle     + d_w;
    //float wristRotNext = hand.angle      + d_wr;
    float wristRotNext = phiNext;
    
    // Hand and Wrist coordinates
    float wristNext_x,  wristNext_y;
    float handNext_x,   handNext_y, handNext_ro, handNext_h;
    float shoulderNext, elbowNext;
    
    
    
//    float wristRotation, wristRotationNext;


    
    // Auxiliar variables
    float diagonal;
    float alfa;                 // ‡ngulo entre la mu–eca y el plano horizontal
    float beta;                 // ‡ngulo entre la el radio del robot y la lunea hasta la mu–ecadel robot (hœmero)

    
    

    // Si el brazo no est‡ demasiado estirado no calcula nada m‡s.
    diagonal = ofDist(shoulder.ro_joint, shoulder.h_joint,wristNext_ro,wristNext_h);
    
    if (diagonal > 1.9 * brazoBotArmLenght){
        printf("*00 Muneca demasiado lejos, %f > %f\n",
               diagonal, 1.9 * brazoBotArmLenght);
        return;
    }
    else{
        // Calcula los ‡ngulos de hombro y codo
        alfa = ofRadToDeg (asin ((wristNext_h - shoulder.h_joint) / diagonal));
        beta = ofRadToDeg (acos ((diagonal/2) / brazoBotArmLenght));
        
        shoulderNext = alfa + beta;
        elbowNext    = 180 - 2 * beta;
        wristNext    = 360-shoulderNext-elbowNext-90;


        // Calculate the new cartesian wrist coordinates
        wristNext_x = wristNext_ro * sin(ofDegToRad(phiNext));
        wristNext_y = wristNext_ro * cos(ofDegToRad(phiNext));

        // Calculate the new hand coordinates
        handNext_ro = wristNext_ro + brazoBotHandLenght *
                    cos(ofDegToRad(360 - shoulderNext - elbowNext - wristNext));
        handNext_h  = wristNext_h  - brazoBotHandLenght *
                    sin(ofDegToRad(360 - shoulderNext - elbowNext - wristNext));
        
        handNext_x = handNext_ro * sin(ofDegToRad(phiNext));
        handNext_y = handNext_ro * cos(ofDegToRad(phiNext));
    }

    
    // Basic robot arm angle coordinates
    if      (phiNext < base.angle_min){
        printf("*01 Arm is too much to the left, %f < %f \n",
                phiNext, base.angle_min);
        base.angle_limits = -2;
        return;
    }
    else if (phiNext > base.angle_Max){
        printf("*02 Arm is too much to the right, %f < %f \n",
                phiNext, base.angle_Max);
        base.angle_limits =  2;
        return;
    }
        
        
    else if (shoulderNext < shoulder.angle_min) {
        printf("*03 shoulder angle is too litle, %f.1 < %f.1 \n",
                shoulderNext, shoulder.angle_min);
        shoulderAngleColor = alertColor2;
        shoulder.angle_limits = -2;
        return;
    }
    else if (shoulderNext > shoulder.angle_Max){
        printf("*04 shoulder angle is too big, %f.1 > %f.1 \n",
                shoulderNext, shoulder.angle_Max);
        shoulderAngleColor = alertColor2;
        shoulder.angle_limits =  2;
        return;
    }
    
        
    else if (elbowNext < elbow.angle_min){
        printf("*05 elbow angle is too litle, %f.1 < %f.1 \n",
                elbowNext, elbow.angle_min);
        elbow.angle_limits =  -2;
        return;
    }
    else if (elbowNext > elbow.angle_Max){
        printf("*06 elbow angle is too big, %f.1 > %f.1 \n",
                elbowNext, elbow.angle_Max);
        elbow.angle_limits =  2;
        return;
    }

    
    else if (wristNext < wrist.angle_min){
        printf("*07 wrist angle is too litle, %f.1 < %f.1 \n",
               wristNext, wrist.angle_min);
        wrist.angle_limits =  -2;
        return;
    }
    else if(wristNext > wrist.angle_Max){
        printf("*08 wrist angle is too big, %f.1 > %f.1 \n",
               wristNext, wrist.angle_Max);
        wrist.angle_limits =  2;
        return;
    }
    
    
    if     (wristRotNext < hand.angle_min){
        printf("*09 wrist rotation angle is too litle, %f.1 < %f.1 \n",
               wristRotNext, hand.angle_min);
        hand.angle_limits = -2;
        return;
    }
    else if(wristRotNext > hand.angle_Max){
        printf("*10 wrist rotation angle is too big, %f.1 > %f.1 \n",
               wristRotNext, hand.angle_Max);
        hand.angle_limits =  2;
        return;
    }

  
    
    if (wristNext_ro < wrist.ro_min){
        printf("*11 wrist is too close to the origin, %f < %f \n",
               wristNext_ro, wrist.ro_min);
        wrist.ro_limits = -2;
        return;
    }
    else if (wristNext_ro > wrist.ro_Max){
        printf("*12 wrist is too far fron the origin, %f > %f \n",
               wristNext_ro, wrist.ro_Max);
        wrist.ro_limits =  2;
        return;
    }
    
    
    if (wristNext_h < wrist.h_min){
        printf("*13 wrist is too low, %f < %f \n",
               wristNext_h, wrist.h_min);
        wrist.h_limits = -2;
        return;
    }
    else if (wristNext_h > wrist.h_Max){
        printf("*14 wrist is too high, %f > %f \n",
               wristNext_h, wrist.h_Max);
        wrist.h_limits =  2;
        return;
    }

    
    if (handNext_ro < hand.ro_min){
        printf("*15 hand is too close to the origin, %f < %f \n",
               handNext_ro, hand.ro_min);
        hand.ro_limits = -2;
        return;
    }
    else if (handNext_ro > hand.ro_Max){
        printf("*16 hand is too far fron the origin, %f > %f \n",
               handNext_ro, hand.ro_Max);
        hand.ro_limits =  2;
        return;
    }
    
    
    if (handNext_h < hand.h_min){
        printf("*17 hand is too low, %f < %f \n",
               handNext_h, hand.h_min);
        hand.h_limits = -2;
        return;
    }
    else if (handNext_h > hand.h_Max){
        printf("*18 hand is too high, %f > %f \n",
               handNext_h, hand.h_Max);
        hand.h_limits =  2;
        return;
    }
    

    else if(wristNext_x < wrist.x_min){
        printf("*19 wrist X is too litle, %f.1 < %f.1 \n",
               wristNext_x, wrist.x_min);
        wrist.x_limits = -2;
        return;
    }
    else if(wristNext_x > wrist.x_Max){
        printf("*20 wrist X is too big, %f.1 < %f.1 \n",
           wristNext_y, wrist.x_Max);
        wrist.x_limits =  2;
        return;
    }


    else if(wristNext_y < wrist.y_min){
        printf("*22 wrist Y is too little, %f.1 > %f.1 \n",
               wristNext_x, wrist.y_min);
        wrist.y_limits = -2;
        return;
    }
    else if(wristNext_y > wrist.y_Max){
        printf("*23 wrist Y is too big, %f.1 > %f.1 \n",
               wristNext_y, wrist.y_Max);
        wrist.y_limits =  2;
        return;
    }

    
 /*

    // Cylindrical wrist coordinates

    // Cylindrical hand coordinates
 

    // Cartesian wrist coordinates

        else if(wristNext_z < bB_Wrist_z_min) printf("*21 wrist Z is too litle, %f.1 < %f.1 \n", wristNext_z, bB_Wrist_z_min);

        else if(wristNext_z > bB_Wrist_z_Max) printf("*24 wrist Z is too big, %f.1 > %f.1 \n", wristNext_z, bB_Wrist_z_Max);

    // Cartesian hand coordinates
        else if(handNext_x < bB_Hand_x_min) printf("*25 hand X is too litle, %f.1 < %f.1 \n", handNext_x, bB_Hand_x_min);
        else if(handNext_y < bB_Hand_y_min) printf("*26 hand Y is too litle, %f.1 < %f.1 \n", handNext_y, bB_Hand_y_min);
        else if(handNext_z < bB_Hand_z_min) printf("*27 hand Z is too litle, %f.1 < %f.1 \n", handNext_z, bB_Hand_z_min);
        
        else if(handNext_x > bB_Hand_x_Max) printf("*28 hand X is too big, %f.1 > %f.1 \n", handNext_x, bB_Hand_x_Max);
        else if(handNext_y > bB_Hand_y_Max) printf("*29 hand Y is too big, %f.1 > %f.1 \n", handNext_y, bB_Hand_y_Max);
        else if(handNext_z > bB_Hand_z_Max) printf("*30 hand Z is too big, %f.1 > %f.1 \n", handNext_z, bB_Hand_z_Max);

  
  */
    
    
    // Actualiza los valores y env’a el comando al brazoBot
    base.angle         = phiNext ;
    
    shoulder.angle     = shoulderNext;
    
    elbow.angle        = elbowNext;
    elbow.ro_joint     = brazoBotArmLenght;
    elbow.h_joint      = brazoBotArmLenght;
            
    wrist.angle        = wristNext;
    wrist.ro_joint     = wristNext_ro;
    wrist.h_joint      = wristNext_h;
    wrist.x_joint      = wristNext_x;
    wrist.y_joint      = wristNext_y;
    
    hand.angle         = wristRotNext;
    hand.ro_joint      = handNext_ro;
    hand.h_joint       = handNext_h;
    

    
    
    
    brazoBotMove (base.angle, shoulder.angle, elbow.angle, wrist.angle, hand.angle, 0);
    
    printf("** Posicion Muneca (x = %f , y = %f , z = %f )\n",
           wrist.ro_joint, wrist.h_joint, wrist.h_joint);
    
    printf("** Posicion Mano (x = %f , y = %f , z = %f )\n",
           hand.ro_joint, hand.h_joint, hand.h_joint);


    printf("** alfa = %f ; beta = %f ; ganma = %f \n\n", alfa, beta, 90.0);

    
    

            /***********************************************************

             //printf("distancia = %f ; distancia plana = %f ; longitud = %f \n", distancia, distanciaPlana,brazoBotLongitud);
             //       printf("** Base= %f, Hombro= %f, Codo= %f, Muneca= %f, Rotacion= %f  \n",
             //              brazoBot_phi, brazoBot_shoulder, brazoBot_elbow, brazoBot_wrist,brazoBot_wristRotation);

            brazoBot_wristRotation = wristRotationNext;
            
           
             ***********************************************************/
            
            
            
            
            
            
    
    
}

//--------------------------------------------------------------
void ofApp::brazoBotMove(float phi,float shoulder,float elbow,float wrist, float wristRotation, float grip){

    unsigned char comando[17];
    
    int phiNext;
    int shoulderNext;
    int elbowNext;
    int wristNext;
    int wristRotationNext;
    int gripNext;
    
    phiNext             = degToServoAX(phi);
    shoulderNext        = degToServoAX(90-18 - shoulder);
    elbowNext           = degToServoAX(elbow -100);
    wristNext           = degToServoAX(wrist -180);
    wristRotationNext   = degToServoAX(wristRotation);
    gripNext            = degToServoAX(grip);
    
    if (brazoBotModo==4){     // Realiza un movimiento en modo BackHoe
        
        comando[0]= 0xFF;                          // Header (0xFF/255)
        
        //anguloBase = max(min(anguloBase,brazoBotBaseMax),brazoBotBaseMin);
        comando[1]=  ((phiNext)/256)%256;           // Base Joint Coordinate High Byte
        comando[2]=  (phiNext)%256;                 // Base Coordinate Low Byte
        
        comando[3]=  ((shoulderNext)/256)%256;      // Shoulder Coordinate High Byte
        comando[4]=  (shoulderNext)%256;            // Shoulder Coordinate Low Byte
        
        comando[5]=  ((elbowNext)/256)%256;         // Elbow Coordinate High Byte
        comando[6]=  (elbowNext)%256;               // Elbow Coordinate Low Byte
        
        
        comando[7]=  ((wristNext)/256)%256;         // Wrist Angle High Byte
        comando[8]=  (wristNext)%256;               // Wrist Angle Low Byte
        
        comando[9]=  ((wristRotationNext)/256)%256; // Wrist Rotate High Byte
        comando[10]= (wristRotationNext)%256;       // Wrist Rotate Low Byte
        
        // Griper = 512
        comando[11]= ((512)/256)%256;               // Griper High Byte
        comando[12]= (512)%256;                     // Gripper Low Byte
        
        // Delta Byte = 128
        comando[13]= 64;                            // Delta Byte
        
        // Button Byte = 0
        comando[14]= 0;                             // Delta Byte
        
        // Extended Instruction Byte = 0
        comando[15]= 0;                             // Delta Byte
        
        // Checksum
        unsigned char checkSum = 0;
        for (int i = 1; i<=15; i++) {checkSum += comando[i];}
        comando[16]= 255 - checkSum % 256;
        
        
        
        
        // Env’a el comando al robot
        if (brazoBotSerial.isInitialized())
            brazoBotSerial.writeBytes(&comando[0], 17);
        
        
        printf(" nueva posicion Base= %f, Hombro= %f, Codo= %f; Muneca= %f, giro =%f, grip =%f)\n",
               phi, shoulder, elbow, wrist, wristRotation, grip);
        
        printf(" nueva posicion Base= %i, Hombro= %i, Codo= %i; Muneca= %i, giro =%i, grip =%i)\n",
               phiNext, shoulderNext, elbowNext, wristNext, wristRotationNext, gripNext);

        printf("comando enviado: %i, %i, %i; W=%i; G=%i; delta=%i ms; check=%i)\n\n",
               comando[1]*256+comando[2],
               comando[3]*256+comando[4],
               comando[5]*256+comando[6],
               comando[7]*256+comando[8],
               comando[9]*256+comando[10],
               comando[13],
               comando[16]);
        
        lastOrderSeconds = ofGetElapsedTimef();

    }
    
    
}


//--------------------------------------------------------------
void ofApp::brazoBotInitBH(){
    
    
//    unsigned long actualTime = ofGetElapsedTimef();
    unsigned char comando[17]={0xff,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x40,0xbf};
   
    
    if (!brazoBotSerial.isInitialized()){
        if (brazoBotSerialInited)
            printf ("Lost serial Port connection \n");
        printf("Robot en modo simulacion. \n");
        brazoBotSerialInited = false;
    }
    else brazoBotSerial.writeBytes(&comando[0], 17);
    
        printf("Robot en modo BackHoe. ");
        printf("comando enviado: %i, %i, %i; W=%i; G=%i; delta=%i ms; check=%i)\n\n",
               comando[1]*256+comando[2],
               comando[3]*256+comando[4],
               comando[5]*256+comando[6],
               comando[7]*256+comando[8],
               comando[9]*256+comando[10],
               comando[13],
               comando[16]);

        brazoBotInited = true;
        brazoBotModo = 4;
 
    base.angle          =  0;
    
    shoulder.angle      = 90;
    shoulder.angle_ant  =  0;
    shoulder.angle_post = 90;
    shoulder.ro_joint   = 0;
    shoulder.h_joint    = 110;
    
    elbow.angle         = 90;
    elbow.angle_ant     = 90;
    elbow.angle_post    = 180;
    elbow.ro_joint      = shoulder.ro_joint;
    elbow.h_joint       = shoulder.h_joint + brazoBotArmLenght;
    
    
    wrist.angle         =  90;
    wrist.angle_ant     = 180;
    wrist.angle_post    = 270;
    wrist.ro_joint      = elbow.ro_joint + brazoBotArmLenght;
    wrist.h_joint       = elbow.h_joint;
    wrist.x_joint       = 0;
    wrist.y_joint       = wrist.ro_joint;
    
    hand.angle          = 0;

    

    // Establece valores iniciales para los movimientos:

    if (brazoBotSerial.isInitialized()) brazoBotDelta_phi_ro_h(0,0,0,0,0);
    
    
    
    /*
     Example 1: Set 3D Cartesian mode / straight wrist and go to home
     0xff 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x20 0xdf
     
     Example 2: Set 3D Cartesian mode / 90¡ wrist angle and go to home
     0xff 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x28 0xd7
     
     Example 3: Set 3D Cylindrical mode / straight wrist and go to home
     0xff 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x30 0xcf
     
     Example 4: Set 3D Cylindrical mode / 90¡ wrist angle and go to home
     0xff 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x38 0xc7
     
     Example 5: Set Backhoe/Joint mode and go to home
     0xff 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x0 0x40 0xbf
     
     Example 19: Center All Servos
     0xff 0x2 0x0 0x2 0x0 0x2 0x0 0x2 0x0 0x2 0x0 0x2 0x0 0x80 0x0 0x0 0x73
     */
    

}

//--------------------------------------------------------------
int ofApp::degToServoAX(float angulo){
    
    int resultado=512;
    
    //if ((angulo>-165)and(angulo<165)) resultado = angulo*1024.0/330.0+512;
    if ((angulo>-165)and(angulo<165)) resultado = angulo*1024.0/308.0+512;
    
    return resultado;
}



