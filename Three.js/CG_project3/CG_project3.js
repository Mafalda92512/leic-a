//global THREE, requestAnimationFrame, console

//const THREE = require("./three");

var camera, camera1, camera2, scene, renderer;

var basicMaterial;

var floor, car, carBodyMesh;

var clock1, timer;

var spot1, spot2, spot3, spotl1, spotl2, spotl3;

var holo1, holo2, holo3;

var rotateL, rotateR;

var directionalLight, ilumCalc, shadingGP, mainLight;

var Windows, Top, Left, Rigth, Lights, Palan, Floor1, wheel1, wheel2,wheel3,wheel4; //MESHES

var type;

//Size e aspectratio para camara ortogonal
var width = 80, height = 60;
var aspectratio;
var ratio = 1.29;
var scale = 0.013;
var scale_width;
var scale_height;
var prev_width;
var prev_height;

var matLambertTruck, matLambertWindow, matLambertLights , matLambertPalanque, matLambertFloor ;
var matPhongTruck, matPhongWindow, matPhongLights, matPhongPalanque, matPhongFloor;
var matBasicTruck, matBasicWindow, matBasicLights, matBasicPalanque, matBasicFloor;

/*======================== SCENE =========================*/
function createFloor(x, y, z) {
    'use strict';

    floor = new THREE.Object3D();
    var geometry = new THREE.CubeGeometry(250, 0.1, 250);
    Floor1 = new THREE.Mesh(geometry, matBasicFloor);
    Floor1.position.set(x, y, z);
    floor.add(Floor1);
    floor.position.set(x, y, z);
    scene.add(floor);

}
function addPalanque(obj, x, y, z) {
    'use strict';

    var geometry = new THREE.CylinderGeometry(75,75 ,3,60);
    Palan = new THREE.Mesh(geometry, matBasicPalanque);
    Palan.position.set(x, y, z);
    obj.add(Palan);

}
function addCarBotom(obj, x, y, z) {
    'use strict';

    var geometry = new THREE.CubeGeometry(60, 0.5, 30);
    var mesh = new THREE.Mesh(geometry, matBasicTruck);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}
function addWheel1(obj, x,y,z) {
    'use strict';

    var geometry = new THREE.CylinderGeometry(8, 8, 4, 20);
    wheel1 = new THREE.Mesh(geometry,  matBasicWindow );
    wheel1.rotateX(Math.PI/2);
    wheel1.position.set(x, y, z);
    obj.add(wheel1);
}
function addWheel2(obj, x,y,z) {
    'use strict';

    var geometry = new THREE.CylinderGeometry(8, 8, 4, 20);
    wheel2 = new THREE.Mesh(geometry,  matBasicWindow );
    wheel2.rotateX(Math.PI/2);
    wheel2.position.set(x, y, z);
    obj.add(wheel2);
}
function addWheel3(obj, x,y,z) {
    'use strict';

    var geometry = new THREE.CylinderGeometry(8, 8, 4, 20);
    wheel3 = new THREE.Mesh(geometry,  matBasicWindow );
    wheel3.rotateX(Math.PI/2);
    wheel3.position.set(x, y, z);
    obj.add(wheel3);
}
function addWheel4(obj, x,y,z) {
    'use strict';

    var geometry = new THREE.CylinderGeometry(8, 8, 4, 20);
    wheel4 = new THREE.Mesh(geometry,  matBasicWindow );
    wheel4.rotateX(Math.PI/2);
    wheel4.position.set(x, y, z);
    obj.add(wheel4);
}
function createLeftPart(obj){
    let left = new THREE.Geometry();

    left.vertices.push(
        new THREE.Vector3( -32 , 9 ,16 ), // v15-0
        new THREE.Vector3( -28 , 16 ,16 ), //v14-1
        new THREE.Vector3( -42 , 12 ,16 ), // v16-2
        new THREE.Vector3( -44 , 21 ,16 ), // v1-3
        new THREE.Vector3( -28 , 33 ,16 ), // v2-4
        new THREE.Vector3( -20 , 15 ,16 ), // v13-5
        new THREE.Vector3( -26 , 30 ,16 ), // v17-6
        new THREE.Vector3( 0 , 43 , 16 ), // v3-7
        new THREE.Vector3( -3 , 39 , 16 ), // v20-8
        new THREE.Vector3( 24 , 30 , 16 ), // v18-9
        new THREE.Vector3( 17 , 35 , 16 ), // v19-10
        new THREE.Vector3( 28 , 30 , 16 ), // v5-11
        new THREE.Vector3( 26.5 , 40 , 16 ), // v4-12
        new THREE.Vector3( -16 , 9 , 16 ), // v12-13
        new THREE.Vector3( 16 , 9 , 16 ), // v11-14
        new THREE.Vector3( 20 , 15 , 16 ), // v10-15
        new THREE.Vector3( 28 , 15 , 16 ), // v9-16
        new THREE.Vector3( 32 , 9 , 16 ), // v8-17
        new THREE.Vector3( 60 , 13 , 16 ), // v7-18
        new THREE.Vector3( 60 , 30 , 16 ) // v6-19

    );

    left.faces.push(

        new THREE.Face3(11, 7, 10),
        new THREE.Face3(0, 1, 2),
        new THREE.Face3(1, 3, 2),
        new THREE.Face3(1, 4, 3),
        new THREE.Face3(1, 5, 6),
        new THREE.Face3(1, 6, 4),
        new THREE.Face3(6, 8, 4),
        new THREE.Face3(4, 8, 7),
        new THREE.Face3(5, 9, 6),
        new THREE.Face3(9,11, 10),
        new THREE.Face3(8, 10, 7),
        new THREE.Face3(11, 12, 7),
        new THREE.Face3(5, 13, 9),
        new THREE.Face3(13, 14, 9),
        new THREE.Face3(14, 15, 9),
        new THREE.Face3(15, 16, 9),
        new THREE.Face3(16,11, 9),
        new THREE.Face3(16, 17, 18),
        new THREE.Face3(16, 18, 19),
        new THREE.Face3(16, 19, 11)


    );

    left.computeFaceNormals();

    Left = new THREE.Mesh( left , matBasicTruck);
    Left.position.set(0,4,0);
    obj.add(Left);
}
function createRightPart(obj){
    let left = new THREE.Geometry();

    left.vertices.push(
        new THREE.Vector3( -32 , 9 ,-16 ), // v15-0
        new THREE.Vector3( -28 , 16 ,-16 ), //v14-1
        new THREE.Vector3( -42 , 12 ,-16 ), // v16-2
        new THREE.Vector3( -44 , 21 ,-16 ), // v1-3
        new THREE.Vector3( -28 , 33 ,-16 ), // v2-4
        new THREE.Vector3( -20 , 15 ,-16 ), // v13-5
        new THREE.Vector3( -26 , 30 ,-16 ), // v17-6
        new THREE.Vector3( 0 , 43 , -16 ), // v3-7
        new THREE.Vector3( -3 , 39 , -16 ), // v20-8
        new THREE.Vector3( 24 , 30 , -16 ), // v18-9
        new THREE.Vector3( 17 , 35 , -16 ), // v19-10
        new THREE.Vector3( 28 , 30 , -16 ), // v5-11
        new THREE.Vector3( 26.5 , 40 , -16 ), // v4-12
        new THREE.Vector3( -16 , 9 , -16 ), // v12-13
        new THREE.Vector3( 16 , 9 , -16 ), // v11-14
        new THREE.Vector3( 20 , 15 , -16 ), // v10-15
        new THREE.Vector3( 28 , 15 , -16 ), // v9-16
        new THREE.Vector3( 32 , 9 , -16 ), // v8-17
        new THREE.Vector3( 60 , 13 , -16 ), // v7-18
        new THREE.Vector3( 60 , 30 , -16 ) // v6-19

    );

    left.faces.push(
        new THREE.Face3(7, 11, 10),
        new THREE.Face3(1, 0, 2),
        new THREE.Face3(3, 1, 2),
        new THREE.Face3(4, 1, 3),
        new THREE.Face3(5, 1, 6),
        new THREE.Face3(6, 1, 4),
        new THREE.Face3(8, 6, 4),
        new THREE.Face3(8, 4, 7),
        new THREE.Face3(9, 5, 6),
        new THREE.Face3(11,9, 10),
        new THREE.Face3(10, 8, 7),
        new THREE.Face3(12, 11, 7),
        new THREE.Face3(13, 5, 9),
        new THREE.Face3(14, 13, 9),
        new THREE.Face3(15, 14, 9),
        new THREE.Face3(16, 15, 9),
        new THREE.Face3(11,16, 9),
        new THREE.Face3(17, 16, 18),
        new THREE.Face3(18, 16, 19),
        new THREE.Face3(19, 16, 11)


    );

    left.computeFaceNormals();

    Right= new THREE.Mesh( left , matBasicTruck);
    Right.position.set(0,4,0);
    obj.add(Right);
}
function createTop(obj){
    let left = new THREE.Geometry();

    left.vertices.push(

        new THREE.Vector3( -44 , 21 ,-16 ), // v1-0
        new THREE.Vector3( -28 , 30 ,-16 ), // v2-1
        new THREE.Vector3( 0 , 43 , -16 ), // v3-2
        new THREE.Vector3( 26.5 , 40 , -16 ), // v4-3
        new THREE.Vector3( 28 , 30 , -16 ), // v5-4
        new THREE.Vector3( 60 , 30 , -16 ), // v6-5

        new THREE.Vector3( -44 , 21 ,16 ), // v42-6
        new THREE.Vector3( -28 , 30 ,16 ), // v41-7
        new THREE.Vector3( 0 , 43 , 16 ), // v40-8
        new THREE.Vector3( 26.5 , 40 , 16 ), // v39-9
        new THREE.Vector3( 28 , 30 , 16 ), // v38-10
        new THREE.Vector3( 60 , 30 , 16 ), // v37-11
        new THREE.Vector3( -50 , 19 , 13 ), // v43-12 *
        new THREE.Vector3( -50 , 19 , -13 ), // v0-13 *
        // Vidro frente
        new THREE.Vector3( -25 , 32 , -13 ), // v45-14
        new THREE.Vector3( -26 , 32 , 13 ), // v46-15
        new THREE.Vector3( -4 , 41 , 13 ), // v47-16
        new THREE.Vector3( -4 , 41 , -13 ), // v44-17
        //vidro tras
        new THREE.Vector3( 26.7 , 37 , -14 ), // v22-18
        new THREE.Vector3( 26.7 , 37 , 14 ), // v23-19
        new THREE.Vector3( 27.5 , 32 , 14 ), // v24-20
        new THREE.Vector3( 27.5 , 32 , -14 ), // v21-21
        //
        new THREE.Vector3( 60 , 13 , -16 ), // v7-22
        new THREE.Vector3( 60 , 13 , 16 ), // v-7-23

        new THREE.Vector3( -50 , 13 ,13 ), // v36-24 *
        new THREE.Vector3( -50 , 13 ,-13 ), // v-7-25 *
        //Farois

        new THREE.Vector3( -50 , 13 ,-9 ), // v-29-26
        new THREE.Vector3( -50 , 13 ,-12 ), // v-30-27
        new THREE.Vector3( -50 , 18 ,-9 ), // v-27-28
        new THREE.Vector3( -50 , 18 ,-12 ), // v-28-29

        new THREE.Vector3( -50 , 13 , 9 ), // v34-30
        new THREE.Vector3( -50 , 13 , 12 ), // v35-31
        new THREE.Vector3( -50 , 18 , 9 ), // v-32-32
        new THREE.Vector3( -50 , 18 , 12 ), // v-33-33
        //
        new THREE.Vector3( -42 , 12 ,-16 ), // v16-34
        new THREE.Vector3( -42 , 12 , 16 ), // v-16-35

    );

    left.faces.push(
        new THREE.Face3(24, 35, 6),
        new THREE.Face3( 24 , 6 , 12),
        new THREE.Face3(34, 25, 0),
        new THREE.Face3( 0 ,25 ,  13),

        new THREE.Face3(29,12,  13),
        new THREE.Face3(33,12,  29),
        new THREE.Face3(31,25,  24),
        new THREE.Face3( 27,25, 31),

        new THREE.Face3(25, 27, 29),
        new THREE.Face3(25,  29 ,13),
        new THREE.Face3(31,24,  12),
        new THREE.Face3(33,31,  12),
        new THREE.Face3( 30,28, 26),
        new THREE.Face3(32, 28, 30),

        new THREE.Face3(4, 3, 21),
        new THREE.Face3(21, 3, 18),
        new THREE.Face3(19,3, 9),
        new THREE.Face3(18,3, 19),

        new THREE.Face3(10,4,  21),
        new THREE.Face3(10,21,  20),

        new THREE.Face3(10,19, 9),

        new THREE.Face3(10,20,  19),

        new THREE.Face3(22, 5 , 11),
        new THREE.Face3(22, 11, 23),

        new THREE.Face3(12, 0, 13),
        new THREE.Face3(6, 0, 12),
        new THREE.Face3(7, 0, 6),
        new THREE.Face3(1, 0, 7),
        new THREE.Face3(2, 8, 9),
        new THREE.Face3(2, 9, 3),
        new THREE.Face3(4, 11, 5),
        new THREE.Face3(10, 11, 4),

        new THREE.Face3(1 , 14 , 2),
        new THREE.Face3(2, 14  , 17),

        new THREE.Face3(2, 17, 16),
        new THREE.Face3(2, 16, 8),
        new THREE.Face3(8, 16, 15),
        new THREE.Face3(8, 15, 7),
        new THREE.Face3(7, 15, 14),
        new THREE.Face3(7 ,14 , 1)

    );

    left.computeFaceNormals();

    Top = new THREE.Mesh( left , matBasicTruck);
    Top.position.set(0,4,0);
    obj.add(Top);
}
function createWindows(obj){
    let left = new THREE.Geometry();

    left.vertices.push(

    // Vidro frente
    new THREE.Vector3( -25 , 32 , -13 ), // v45-0
    new THREE.Vector3( -26 , 32 , 13 ), // v46-1
    new THREE.Vector3( -4 , 41 , 13 ), // v47-2
    new THREE.Vector3( -4 , 41 , -13 ), // v44-3
    //vidro tras
    new THREE.Vector3( 26.7 , 37 , -14 ), // v22-4
    new THREE.Vector3( 26.7 , 37 , 14 ), // v23-5
    new THREE.Vector3( 27.5 , 32 , 14 ), // v24-6
    new THREE.Vector3( 27.5 , 32 , -14 ), // v21-7

    new THREE.Vector3( -26 , 30 ,-16 ), // v17-8
    new THREE.Vector3( -3 , 39 , -16 ), // v20-9
    new THREE.Vector3( 17 , 35 , -16 ), // v19-10
    new THREE.Vector3( 24 , 30 , -16 ), // v18-11

    new THREE.Vector3( -26 , 30 ,16 ), // v17-12
    new THREE.Vector3( -3 , 39 , 16 ), // v20-13
    new THREE.Vector3( 17 , 35 , 16 ), // v19-14
    new THREE.Vector3( 24 , 30 , 16 ) // v18-15
    )


    left.faces.push(

        new THREE.Face3(0, 1, 2),
        new THREE.Face3( 0 , 2 , 3),
        new THREE.Face3(4, 5, 7),
        new THREE.Face3( 7 , 5 ,  6),

        new THREE.Face3(8 ,9, 11),
        new THREE.Face3(9 ,10,  11),
        new THREE.Face3(12 ,15, 13),
        new THREE.Face3(15 ,14,  13)

    );

    left.computeFaceNormals();

    Windows = new THREE.Mesh( left , matBasicWindow);
    Windows.position.set(0,4,0);
    obj.add(Windows);
}
function createfrontLights(obj){
    let left = new THREE.Geometry();

    left.vertices.push(

        new THREE.Vector3( -50 , 13 ,-9 ), // v-29-26 0
        new THREE.Vector3( -50 , 13 ,-12 ), // v-30-27 1
        new THREE.Vector3( -50 , 18 ,-9 ), // v-27-28 2
        new THREE.Vector3( -50 , 18 ,-12 ), // v-28-29 3

        new THREE.Vector3( -50 , 13 , 9 ), // v34-30 4
        new THREE.Vector3( -50 , 13 , 12 ), // v35-31 5
        new THREE.Vector3( -50 , 18 , 9 ), // v-32-32 6
        new THREE.Vector3( -50 , 18 , 12 ), // v-33-33 7
    )


    left.faces.push(

        new THREE.Face3(0,3,  1),
        new THREE.Face3( 2 ,3,  0),
        new THREE.Face3(4, 5, 6),
        new THREE.Face3( 6 , 5 ,  7),


    );

    left.computeFaceNormals();

    Lights = new THREE.Mesh( left , matBasicLights);
    Lights.position.set(0,4,0);
    obj.add(Lights);
}
function createCar(x,y,z) {
    'use strict';

    car = new THREE.Object3D();
    addPalanque(car, 0,1.5,0);
    addCarBotom(car, 0,9+4,0);
    addWheel1(car, -25, 9+4, 16);
    addWheel2(car, -25, 9+4, -16);
    addWheel3(car, 25, 9+4, 16);
    addWheel4(car, 25, 9+4, -16);
    createLeftPart(car);
    createRightPart(car);
    createTop(car);
    createWindows(car);
    createfrontLights(car);
    car.position.set(x, y, z);
    scene.add(car);
}
function changeTudoLambert(){
    Windows.material = matLambertWindow ;
    Top.material = matLambertTruck;
    Left.material = matLambertTruck;
    Right.material = matLambertTruck;
    Lights.material = matLambertLights;
    Palan.material = matLambertPalanque;
    Floor1.material = matLambertFloor;
    wheel1.material = matLambertWindow ;
    wheel2.material = matLambertWindow ;
    wheel3.material = matLambertWindow ;
    wheel4.material = matLambertWindow ;
}
function changeTudoPhong(){
    Windows.material = matPhongWindow ;
    Top.material = matPhongTruck;
    Left.material = matPhongTruck;
    Right.material = matPhongTruck;
    Lights.material = matPhongLights;
    Palan.material = matPhongPalanque;
    Floor1.material = matPhongFloor;
    wheel1.material = matPhongWindow ;
    wheel2.material = matPhongWindow ;
    wheel3.material = matPhongWindow ;
    wheel4.material = matPhongWindow ;
}
function changeTudoBasic(){
    Windows.material = matBasicWindow ;
    Top.material = matBasicTruck;
    Left.material = matBasicTruck;
    Right.material = matBasicTruck;
    Lights.material = matBasicLights;
    Palan.material = matBasicPalanque;
    Floor1.material = matBasicFloor;
    wheel1.material = matBasicWindow ;
    wheel2.material = matBasicWindow ;
    wheel3.material = matBasicWindow ;
    wheel4.material = matBasicWindow ;
}

function createScene() {
    'use strict';

    scene = new THREE.Scene();
    //scene.add(new THREE.AxesHelper(10));
    //createMaterials();
    //createFloor(0, 0, 0);
    //createCar(0,0,0);
    //createLights();
}
function createMaterials(){
    matLambertTruck = new THREE.MeshLambertMaterial({color: 0x808080, wireframe:false, side: THREE.DoubleSide});
    matPhongTruck = new THREE.MeshPhongMaterial({color: 0x808080, wireframe:false, specular:0x111511, side: THREE.DoubleSide});
    matPhongTruck.shininess = 60;
    matBasicTruck = new THREE.MeshBasicMaterial( {color: 0x808080, wireframe:false, side: THREE.DoubleSide });

    matLambertWindow = new THREE.MeshLambertMaterial({color: 0x090000, wireframe:false, side: THREE.DoubleSide});
    matPhongWindow = new THREE.MeshPhongMaterial({color: 0x090000, wireframe:false, specular:0x111511, side: THREE.DoubleSide});
    matPhongWindow.shininess = 60;
    matBasicWindow = new THREE.MeshBasicMaterial( {color: 0x090000, wireframe:false, side: THREE.DoubleSide });

    matLambertLights = new THREE.MeshLambertMaterial({color: 0xfdffc7, wireframe:false, side: THREE.DoubleSide });
    matPhongLights = new THREE.MeshPhongMaterial({color: 0xfdffc7, wireframe:false, specular:0x111511, side: THREE.DoubleSide});
    matPhongLights.shininess = 60;
    matBasicLights = new THREE.MeshBasicMaterial( {color: 0xfdffc7, wireframe:false, side:THREE.DoubleSide });

    matLambertPalanque = new THREE.MeshLambertMaterial({color: 0x0a00e0, wireframe:false, side: THREE.DoubleSide});
    matPhongPalanque = new THREE.MeshPhongMaterial({color: 0x0a00e0, wireframe:false, specular:0x111511, side: THREE.DoubleSide});
    matPhongPalanque.shininess = 60;
    matBasicPalanque = new THREE.MeshBasicMaterial( {color: 0x0a00e0, wireframe:false, side: THREE.DoubleSide });

    matLambertFloor = new THREE.MeshLambertMaterial({color: 0x00AA55, wireframe:false, side: THREE.DoubleSide});
    matPhongFloor = new THREE.MeshPhongMaterial({color: 0x00AA55, wireframe:false, specular:0x111511, side: THREE.DoubleSide});
    matPhongFloor.shininess = 60;
    matBasicFloor = new THREE.MeshBasicMaterial( {color: 0x00AA55, wireframe:false, side: THREE.DoubleSide });
}

/*========================= LIGHTS ========================*/
function createMainLight(){
    directionalLight=true;
    mainLight = new THREE.DirectionalLight( 0xffffff, 0.6 );
    scene.add(mainLight);
}
function createSpotlight( n ){
    if(n==1) holo1=true;
    else if(n==2) holo2=true;
    else if(n==3) holo3=true;

    basicMaterial = new THREE.MeshBasicMaterial({color: 0x2c2c2c, wireframe: false});
    let spotObj = new THREE.Object3D();
    let geometry1 = new THREE.SphereGeometry(2.5, 20, 20);
    let mesh1 = new THREE.Mesh(geometry1 , basicMaterial);
    let geometry = new THREE.ConeGeometry(4, 5, 20);
    let mesh = new THREE.Mesh(geometry, basicMaterial);
    mesh1.position.set( 0, 0, 0);
    mesh.position.set(0, 0,2);
    mesh.rotateX(Math.PI/2);
    spotObj.add(mesh);
    spotObj.add(mesh1);

    if (n==2) {
        spotObj.position.set(-60, 20 ,60);
        spot2=spotObj;
        spot2.lookAt(new THREE.Vector3(-100,0,100));
        spotl2 = new THREE.SpotLight(0xffffff);
        spotl2.position.set(-60,20,60);

        scene.add(spotl2);
        scene.add(spot2);
    }
    else if (n==1) {
        spotObj.position.set(60,20,60);
        spot1=spotObj;
        spot1.lookAt(new THREE.Vector3(100,0,100));
        spotl1 = new THREE.SpotLight(0xffffff);
        spotl1.position.set(60,20,60);

        scene.add(spotl1);
        scene.add(spot1);
    }
    else if (n==3) {
        spotObj.position.set(60, 20, -60);
        spot3=spotObj;
        spot3.lookAt(new THREE.Vector3(100,0,-100));
        spotl3 = new THREE.SpotLight(0xffffff);
        spotl3.position.set(60,20,-60);

        scene.add(spotl3);
        scene.add(spot3);
    }


}
function createLights(){
    createMainLight();
    createSpotlight(1);
    createSpotlight(2);
    createSpotlight(3);
}

/*========================= CAMERAS =======================*/

function createCameras(){ // cria as camaras 1 e 2
    'use strict'
    createCamera1();
    createCamera2();
    camera = camera1; // define a camera inicial como a camera1
}
function createCamera1(){ // criacao camara perspetiva com vista de topo
    'use strict';

    camera1 = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 1000);
    // field of view, aspect ratio, near, far
    camera1.position.x = 100;
    camera1.position.z = 100;
    camera1.position.y = 100;
    camera1.lookAt(scene.position);
}
function createCamera2() {   // criacao camara ortogonal alinhada com o referencial do palanque
    'use strict';

    if (window.innerWidth / window.innerHeight > ratio){            //se for maior que o ratio dividimos pela scale_height
        camera2 = new THREE.OrthographicCamera(-window.innerWidth / scale_height, window.innerWidth / scale_height, -window.innerHeight / scale_height, window.innerHeight / scale_height, 1, 1000);

    }else {                                                         //caso contrario dividimos pela scale_width
        camera2= new THREE.OrthographicCamera(-window.innerWidth / scale_width, window.innerWidth / scale_width, -window.innerHeight / scale_width, window.innerHeight / scale_width, 1, 1000);
    }

    prev_width = window.innerWidth;
	prev_height = window.innerHeight;

    camera2.position.x = 0;
    camera2.position.z = 90;
    camera2.position.y = 20;
    camera2.lookAt(scene.position);
    camera2.rotation.z=Math.PI;
}

function changeToCamera1(){
    'use strict'
    camera = camera1;
}
function changeToCamera2(){
    'use strict'
    camera = camera2;
}

/*======================== EVENTOS ======================*/
function onResize() {    //atualiza o tamanho da janela
    'use strict';

    if (camera==camera1){                                           //Resize da camera perspetiva
        renderer.setSize(window.innerWidth, window.innerHeight);

        if (window.innerHeight > 0 && window.innerWidth > 0) {
            camera.aspect = window.innerWidth / window.innerHeight;
            camera.updateProjectionMatrix();
        }
    }
                                                                    //Resize da camara ortogonal
    if (camera==camera2){
        renderer.setSize(window.innerWidth, window.innerHeight);
                                                                        //atualizar scale de acordo com medidas anteriores
        scale_width = (window.innerWidth * scale_width) / prev_width;
        scale_height = (window.innerHeight * scale_height) / prev_height;

        prev_width = window.innerWidth;
        prev_height = window.innerHeight;

                                                                        //atualizar novas medidas de acordo com nova scale
        if (window.innerWidth / window.innerHeight > ratio){
            camera.left = -window.innerWidth / scale_height;
            camera.right = window.innerWidth / scale_height;
            camera.top = -window.innerHeight / scale_height;
            camera.bottom = window.innerHeight / scale_height;
            camera.updateProjectionMatrix();
        }
        else {
            camera.left = -window.innerWidth / scale_width;
            camera.right = window.innerWidth / scale_width;
            camera.top = -window.innerHeight / scale_width;
            camera.bottom = window.innerHeight / scale_width;
            camera.updateProjectionMatrix();
        }

    }
}
function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {

    case 49: //tecla 1 - ativa/desativa holofote 1
        holo1 = !holo1;
        break;
    case 50: //tecla 2 - ativa/desativa holofote 2
        holo2 = !holo2;
        break;
    case 51: //tecla 3 - ativa/desativa holofote 3
        holo3 = !holo3;
        break;
    case 52: // tecla 4 - camera com projecao perspetiva
        changeToCamera1();
        break;
    case 53: // tecla 5 - camera com projecao ortogonal
        changeToCamera2();
        break;
    case 37: // seta para esquerda - roda o palanque para a esquerda
        rotateL = true;
        break;
    case 39: // seta para direita - roda o palanque para a direita
        rotateR = true;
        break;
    case 81: // Q - liga/desliga fonte de luz direcional
    case 113: // q
        directionalLight = !directionalLight;
        break;
    case 87: // W - ativa/desativa calculo iluminacao
    case 119: // w
        if(ilumCalc == 1 ){ // desligado
            ilumCalc = 2 ; // ligado
        }
        else if(ilumCalc == 2 ){ // ligado
            ilumCalc = 1 ; // desligado
            shadingGP = 0; // desliga-se tambem o sombreamento
        }
        else{ // se ilumCalc tiver a 0 - esta ligado e ja atualizou o shading
            ilumCalc = 1 // quando se carrega, desliga-se o calculo
            shadingGP = 0;
        }

        break;
    case 69: // E - alterna sombreamento
    case 101: // e
        if(shadingGP == 1 ){ // lambert to phong
            shadingGP = 2 ;
        }
        else if(shadingGP == 2 ){ // phong to lambert
            shadingGP = 1 ;
        }
        else{
            shadingGP = 0; // quando o calculo da iluminacao esta desligado
        }

            break;
    }

}
function onKeyUp(e) {
    'use strict';

    switch (e.keyCode) {

    case 37: // seta para esquerda
        rotateL = false;
        break;
    case 39: // seta para direita
        rotateR = false;
        break;
    }
}

/*====================== FUNCOES CALLBACK =====================*/

function RotatePalanqueTruck( variacao ){ /* -> <- utilizados para rodar o paranque e o truck */
    if (rotateL) {
        car.rotateY(1 * variacao);
    }
    else if (rotateR) {
        car.rotateY(-1 * variacao);
    }
    else{ /* Do nothing */}
}

function switchMainLight(){ /* Apagar e Ligar a luz principal "Q"*/
    mainLight.visible=directionalLight   ;
}

function calculateMainLight(){ /* Ativar e desativar o calculo da iluminacao w */
    if(ilumCalc==1){
        changeTudoBasic(); // calculo iluminacao desligado
    }
    else if( ilumCalc == 2 ){
        if(type==1) // guarda o shading quando a iluminacao esta desigada
            shadingGP=1;
        else
            shadingGP=2;
        ilumCalc=0; // nao e necessario alteracoes enquanto nao se liga/desliga de novo a iluminacao
    }
    else{
    }

}

function changeMainLightShadingType(){ /* between Gourad(difuse) and Phong*/

    if(shadingGP==1){
        changeTudoLambert();
        type=1; // guarda o shading (para caso o calculo da iluminacao se desligue)
    }
    else if(shadingGP ==2 ){
        changeTudoPhong();
        type=2;

    }
    else{ // caso em que shadingGP == 0 -> calculo iluminacao desligado

    }

}

function turnOffOnSpotlight( ){
    spotl1.visible = holo1; // torna os holofotes visiveis/invisiveis
    spotl2.visible = holo2;
    spotl3.visible = holo3;

}


/*======================= LOOP PROGRAMA ========================*/
function render() {
    'use strict';

    renderer.render(scene, camera);
}
function animate() {
    'use strict';

    var variacao;
    variacao = clock1.getDelta(); // tempo que passou desde a ultima chamada
    //Update
    RotatePalanqueTruck(variacao );
    //switchMainLight(); // apaga/liga luz principal
    //turnOffOnSpotlight( ); // apaga/liga holofotes
    //calculateMainLight();
    // caso o calculo esteja ligado, recupera o tipo de shading a ser utilizado
    // caso esteja desligado, altera os materiais para basic

    //changeMainLightShadingType();
    // caso o calculo da iluminacao esteja ligado
    // vai alterar materiais utilizados de acordo com o shading correspondente,

    //Display
    render(); // atualizacao imagem
    requestAnimationFrame(animate);
}

function init() {
    'use strict'; // nao permite utilizar variaveis nao declaradas

    renderer = new THREE.WebGLRenderer({ // gerador da imagem
        antialias: true // reducao do efeito de serrilhamento
    });

    clock1 = new THREE.Clock(); // temporizador
    clock1.start();

    renderer.setSize(window.innerWidth, window.innerHeight); // definicao tamanho janela
    scale_width = window.innerWidth * scale;
	scale_height = window.innerHeight * scale * ratio;

    document.body.appendChild(renderer.domElement); // adicionar o elemento renderer ao doc HTML
    shadingGP=0;
    ilumCalc=1;
    type=1;
    createScene(); // criacao cena
    createCameras(); // criacao das cameras 1, 2 e 3


    render(); // imagem resultante

    window.addEventListener("keydown", onKeyDown); // espera de eventos
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);
}
