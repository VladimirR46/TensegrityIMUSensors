    import processing.serial.*;
    import toxi.geom.*;
    import toxi.processing.*;
    import processing.net.*;
    
    Server myServer;
     
    // The serial port
    Serial port;                         
     
    String message;
     
    float[] q = new float[4];
    float[] q2 = new float[4];
    float[] q3 = new float[4];
    Quaternion quat = new Quaternion(1, 0, 0, 0);
    Quaternion quat2 = new Quaternion(1, 0, 0, 0);
    Quaternion quat3 = new Quaternion(1, 0, 0, 0);
    // new line character in ASCII
    int newLine = 13; 
    String [] massQ = new String [4];
    float[] ypr = new float[3];
     
    void setup() 
    {
      // size form 300x300
      size(800, 800, P3D); 
      // open serial port
      // replace "COM29" with the COM port on which your arduino is connected
      //port = new Serial(this, "/dev/ttyUSB0", 115200);
      myServer = new Server(this, 4013); 
      // wait init Serial for Arduino
      delay(1000);
    }
     
    void draw() 
    {
      // read and parse incoming serial message
      serialEvent();
      // set background to black
      background(0);
      printQuaternions();
      printYawPitchRoll();
      // set position to centre
      translate(width / 2, height / 2);
      // begin object
      pushMatrix();
      float[] axis = quat.toAxisAngle();
      rotate(axis[0], axis[2], axis[3], axis[1]);
      // draw main body in red
      drawBody();
      // draw front-facing tip in blue
      drawCylinder();
      // draw Triangles
      drawTriangles();
      // draw Quards
      drawQuards();
      // end of object
      
      popMatrix();
      
      translate(width / 4, height / 4);
      // begin object
      pushMatrix();
      float[] axis2 = quat2.toAxisAngle();
      rotate(axis2[0], axis2[2], axis2[3], axis2[1]);
      // draw main body in red
      drawBody();
      // draw front-facing tip in blue
      drawCylinder();
      // draw Triangles
      drawTriangles();
      // draw Quards
      drawQuards();

      
      popMatrix();
      
      translate(width / 8, height / 8);
      // begin object
      pushMatrix();
      float[] axis3 = quat3.toAxisAngle();
      rotate(axis3[0], axis3[2], axis3[3], axis3[1]);
      // draw main body in red
      drawBody();
      // draw front-facing tip in blue
      drawCylinder();
      // draw Triangles
      drawTriangles();
      // draw Quards
      drawQuards();

      
      popMatrix();
      
      // send 's' to Arduino
      //port.write('s');
    }
     
    void serialEvent() { // Function for parsing serial message
      // read from port until new line (ASCII code 13)
      
        Client thisClient = myServer.available();
  // If the client is not null, and says something, display what it said
  if (thisClient !=null) {
    String whatClientSaid = thisClient.readString();
    //println(whatClientSaid); 
    if (whatClientSaid != null)
    {
      //println(whatClientSaid);  
      massQ = split(whatClientSaid, " ");
        q[0] = float(massQ[0]);
        q[1] = float(massQ[1]);
        q[2] = float(massQ[2]);
        q[3] = float(massQ[3]);
        
        q2[0] = float(massQ[4]);
        q2[1] = float(massQ[5]);
        q2[2] = float(massQ[6]);
        q2[3] = float(massQ[7]);
        
        q3[0] = float(massQ[8]);
        q3[1] = float(massQ[9]);
        q3[2] = float(massQ[10]);
        q3[3] = float(massQ[11]);
    } 
  } 
      
      //message = port.readStringUntil(newLine);
      
      /*
      if (message != null) {
        // split message by commas and store in String array 
        massQ = split(message, ",");
        q[0] = float(massQ[0]);
        q[1] = float(massQ[1]);
        q[2] = float(massQ[2]);
        q[3] = float(massQ[3]);
      }
      // print values to console
      print(q[0]);
      print("\t");
      print(q[1]); 
      print("\t");
      print(q[2]);   
      print("\t");
      print(q[3]);   
      println("\t");
      */
      // set our toxilibs quaternion to new data
      quat.set(q[0], q[1], q[2], q[3]);
      quat2.set(q2[0], q2[1], q2[2], q2[3]);
      quat3.set(q3[0], q3[1], q3[2], q3[3]);
     
    }
     
    void drawCylinder() {
      float topRadius = 0;
      float bottomRadius = 20;
      float tall = 20;
      int sides = 8;
      // begin object
      pushMatrix();
      translate(0, 0, -120);
      rotateX(PI/2);
      fill(0, 0, 255, 200);
     
      float angle = 0;
      float angleIncrement = TWO_PI / sides;
      beginShape(QUAD_STRIP);
      for (int i = 0; i < sides + 1; ++i) {
        vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
        vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
        angle += angleIncrement;
      }
      endShape();
     
      // if it is not a cone, draw the circular top cap
      if (topRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
     
        // center point
        vertex(0, 0, 0);
        for (int i = 0; i < sides + 1; i++) {
          vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
          angle += angleIncrement;
        }
        endShape();
      }
     
      // if it is not a cone, draw the circular bottom cap
      if (bottomRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
     
        // center point
        vertex(0, tall, 0);
        for (int i = 0; i < sides + 1; i++) {
          vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
          angle += angleIncrement;
        }
       endShape();
      }
      popMatrix(); 
    }
     
    void drawBody() {
      fill(255, 0, 0, 200);
      box(10, 10, 200);
    }
     
    void drawTriangles() {
      // draw wings and tail fin in green
      fill(0, 255, 0, 200);
      beginShape(TRIANGLES);
      // wing top layer
      vertex(-100,  2, 30); vertex(0,  2, -80); vertex(100,  2, 30);
      // wing bottom layer
      vertex(-100, -2, 30); vertex(0, -2, -80); vertex(100, -2, 30);
      // tail left layer
      vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);
      // tail right layer
      vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);
      endShape();
    }
     
    void drawQuards() {
      beginShape(QUADS);
      vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
      vertex( 100, 2, 30); vertex( 100, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
      vertex(-100, 2, 30); vertex(-100, -2, 30); vertex(100, -2,  30); vertex(100, 2,  30);
      vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2, -30, 98); vertex(-2, -30, 98);
      vertex(-2,   0, 98); vertex(2,   0, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
      vertex(-2, -30, 98); vertex(2, -30, 98); vertex(2,   0, 70); vertex(-2,   0, 70);
      endShape();
    }
     
    void printQuaternions() {
      textMode(SHAPE); // set text mode to shape
      textSize(13);
      fill(255, 255, 255);
      text("Quaternions:", 20, 20, 10);
      text(q[0], 20, 40, 10);
      text(q[1], 20, 60, 10);
      text(q[2], 20, 80, 10);
      text(q[3], 20, 100, 10);
    }
     
    void printYawPitchRoll() {
      // calculate yaw/pitch/roll angles
      ypr[0] = atan2(2 * q[1] * q[2] - 2 * q[0] * q[3], 2 * q[0] * q[0] + 2 * q[1] * q[1] - 1)*57.2;
      ypr[1] = atan2(2 * q[2] * q[3] - 2 * q[0] * q[1], 2 * q[0] * q[0] + 2 * q[3] * q[3] - 1)*57.2;
      ypr[2] = -atan2(2.0f * (q[0] * q[2] - q[1] * q[3]), 1.0f - 2.0f * (q[2] * q[2] + q[1] * q[1]))*57.2;
     
      text("Yaw:", 150, 20, 10);
      text(ypr[0], 150, 40, 10);
      text("Pitch:", 220, 20, 10);
      text(ypr[1], 220, 40, 10);
      text("Roll:", 290, 20, 10);
      text(ypr[2], 290, 40, 10);
    }
