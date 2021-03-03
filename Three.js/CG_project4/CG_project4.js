//global THREE, requestAnimationFrame, console

//const THREE = require("./three");

var posX, posY, posZ, pos0X, pos0Y, pos0Z;

var camera, camera1, camera2, scene, renderer, scene1;

var clock1, timer, pause_block;

var directional_L, point_L, ilumCalc;

var meshBall, meshGreen, meshTriangle, meshStick, flag;

var pauseScreen;

var reset_flag=0;

var controls;

//Size e aspectratio para camara ortogonal
var widh = 80, height = 60;
var aspectratio;
var ratio = 1.29;
var scale = 0.013;
var scale_width;
var scale_height;
var prev_width;
var prev_height;


//textures
var grass = 'textures/grass.png';
var grasstop = 'textures/grasstop.png';
var ball = 'textures/ball.png';
var pscreen = 'textures/pause.png';
var pause, pause_flag, ball_flag, wf_flag;

/*======================== SCENE =========================*/
function createGreen(){
    var geometry= new THREE.PlaneGeometry(100,100, 0);
    var texture = new THREE.TextureLoader().load(grass);  //padrao axadrezado

    texture.wrapS = THREE.RepeatWrapping;
    texture.wrapT = THREE.RepeatWrapping;
    texture.repeat.set(20, 20);
    var texture2 = new THREE.TextureLoader().load(grasstop); // Visao de cima para profundidade de bumpmap

    texture2.wrapS = THREE.RepeatWrapping;
    texture2.wrapT = THREE.RepeatWrapping;
    texture2.repeat.set(20, 20);
    var material1 = new THREE.MeshPhongMaterial( { map: texture , specular:0x11111, shininess:25, wireframe : false} );

    material1.bumpMap = texture2; // Visao de cima para profundidade de bumpmap
    material1.bumpScale = 15;


    var material2 = new THREE.MeshBasicMaterial( { map: texture , wireframe : false, color: 0x00AA00} );

    meshGreen = new THREE.Mesh(geometry, material2);
    meshGreen.userData=  {phong: material1 , basic: material2};

    meshGreen.material.depthTest = true;
    meshGreen.material.depthWrite = true;
    meshGreen.rotateX(- Math.PI/2);
    meshGreen.position.set(0,0,0);
    scene.add(meshGreen);
}
function createBall(){
    var geometry= new THREE.SphereGeometry(2,20,20);
    var texture = new THREE.TextureLoader().load(ball);
    texture.wrapS = THREE.ClampToEdgeWrappin;
    texture.wrapT = THREE.ClampToEdgeWrappin;
    texture.repeat.set(1, 1);

    var material1 = new THREE.MeshPhongMaterial( { specular : 0x111111 , wireframe : false, shininess: 25} );    //specular very dark grey
    material1.bumpMap=texture;
    material1.bumpScale = 5;
    var material2 = new THREE.MeshBasicMaterial({color: 0xfffaf0 , wireframe : false});

    meshBall = new THREE.Mesh(geometry, material2);
    meshBall.userData = {phong: material1 , basic:material2, step: 0} ;
    meshBall.position.set(0,2,0);
    pos0X=0;
    pos0Y=2;
    pos0Z=0;
    posY=2;
    meshBall.material.depthTest = true;
    meshBall.material.depthWrite = true;

    scene.add(meshBall);
}
function createFlag(){
    flag = new THREE.Object3D();
    var material1 = new THREE.MeshPhongMaterial( {wireframe : false , color: 0xf0f0f0,side: THREE.DoubleSide} );
    var material2 = new THREE.MeshBasicMaterial( { wireframe : false , color: 0xf0f0f0,side: THREE.DoubleSide});
    var geometry1 = new THREE.CylinderGeometry(0.8, 0.8, 19);
    meshStick = new THREE.Mesh(geometry1 , material2);
    meshStick.userData = {phong:material1 , basic:material2};
    flag.add(meshStick);

    var geometry2 = new THREE.Geometry();
    var a = new THREE.Vector3(10, 8,0);
    var b = new THREE.Vector3(0,10,0);
    var c = new THREE.Vector3(0,4,0);
    geometry2.vertices.push( a ,
                        b ,
                        c
                     );


    geometry2.faces.push( new THREE.Face3(1, 0, 2),
                    new THREE.Face3(0, 1, 2)
                    );

  var material3 = new THREE.MeshPhongMaterial( {wireframe : false , color: 0xB22222});
  var material4 = new THREE.MeshBasicMaterial( { wireframe : false , color: 0xB22222});

  geometry2.computeFaceNormals();

  meshTriangle= new THREE.Mesh(geometry2, material4);
  meshTriangle.userData = {phong:material3 , basic:material4};

  flag.add(meshTriangle);
  flag.position.set(-37, 7.5, -43);
  scene.add(flag);
}
function createPauseScreen(){
  var texture = new THREE.TextureLoader().load(pscreen);
  var material = new THREE.MeshBasicMaterial({ map: texture });
  var geometry = new THREE.PlaneGeometry( 100, 50 , 0);

  pauseScreen= new THREE.Mesh(geometry , material);
  pauseScreen.visible=true;
  pauseScreen.rotateZ(Math.PI);

  scene1.add(pauseScreen);
}
function createScene() {
  scene = new THREE.Scene();
  createGreen();
  createBall();
  createFlag();
  createLights();
  scene.background = new THREE.CubeTextureLoader()
	.setPath( 'textures/' )
	.load( [
		'px.png',
		'nx.png',
		'py.png',
		'ny.png',
		'pz.png',
		'nz.png'
	] );

}
function createScene1(){
    //mesh comeca invisible
    scene1 = new THREE.Scene();

    createPauseScreen();


}
/*========================= LIGHTS ========================*/
function createDirectionalLight(){
    directional_L=true;
    directionalLight= new THREE.DirectionalLight( 0xffffff, 0.9 );
    directionalLight.position.set( 10 , 20 , 20);
    scene.add(directionalLight);
}
function createPointLight(){
    point_L=true;
    pointLight= new THREE.PointLight(0xffffff, 2.5, 100);
    pointLight.position.set( 25, 20, 25 );
    scene.add( pointLight );
}
function createLights(){
    createDirectionalLight();
    createPointLight();
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

    camera1.position.x = 50;
    camera1.position.z = 50;
    camera1.position.y = 80;
    camera1.lookAt(scene.position);

    controls = new THREE.OrbitControls( camera1, renderer.domElement );
}
function createCamera2(){   // criacao camara ortogonal alinhada com o referencial do palanque
    'use strict';

        if (window.innerWidth / window.innerHeight > ratio){            //se for maior que o ratio dividimos pela scale_height
            camera2 = new THREE.OrthographicCamera(-window.innerWidth / scale_height, window.innerWidth / scale_height, -window.innerHeight / scale_height, window.innerHeight / scale_height, 1, 1000);

        }else {                                                         //caso contrario dividimos pela scale_width
            camera2= new THREE.OrthographicCamera(-window.innerWidth / scale_width, window.innerWidth / scale_width, -window.innerHeight / scale_width, window.innerHeight / scale_width, 1, 1000);
        }

        prev_width = window.innerWidth;
        prev_height = window.innerHeight;

    camera2.position.z = -60;

    scene1.add(camera2);
    camera2.lookAt(scene1.position);
   // camera2.position.x = 0;
    //camera2.position.z = 90;
    //camera2.position.y = 20;
   // camera2.rotation.z=Math.PI;
}
function changeToCamera1(){
    'use strict'
    camera = camera1;
}
function changeToCamera2(){
    'use strict'
    camera = camera2;
}
/*======================== FUNCOES CALLBACK ======================*/
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

    case 82: // R - refresh/reset
    case 114: // r
        reset_flag = 1 ;
        break;
    case 83: // S - pausa/retomar
    case 115: // s
        pause = !pause;
        break;
    case 87: // W - liga/desliga wireframe
    case 119: // w
        wf_flag = !wf_flag;
        break;
    case 66: // B - para/inicia bola de golfe
    case 98: // b
        ball_flag = !ball_flag;
        break;
    case 73: // I - ativa/desativa calculo iluminacao
    case 105: // i
        ilumCalc = !ilumCalc;
        break;
    case 68: // D - liga/desliga luz direcional
    case 100: // d
        directional_L = !directional_L;
        break;
    case 80: // P - liga/desliga luz pontual
    case 112: // p
        point_L = !point_L;
        break;
    }

}
/*====================== ATUALIZAR CENA =====================*/
function changePhong(){
    meshGreen.material = meshGreen.userData.phong;
    meshBall.material = meshBall.userData.phong;
    meshTriangle.material = meshTriangle.userData.phong;
    meshStick.material = meshStick.userData.phong;
}
function changeBasic(){
    meshGreen.material = meshGreen.userData.basic;
    meshBall.material = meshBall.userData.basic;
    meshTriangle.material = meshTriangle.userData.basic;
    meshStick.material = meshStick.userData.basic;
}
function updateWireframe(){
    meshGreen.material.wireframe = wf_flag;
    meshBall.material.wireframe =wf_flag;
    meshTriangle.material.wireframe = wf_flag;
    meshStick.material.wireframe = wf_flag;
}
function switchDirectionalLight(){ /* Apagar e Ligar a luz direcional "D"*/
    if (ilumCalc) {
        directionalLight.visible=directional_L;
    }
}
function switchPointLight(){ /* Apagar e Ligar a luz direcional "D"*/
    if (ilumCalc) {
        pointLight.visible=point_L;
    }
}
function calculateLight(){ /* Ativar e desativar o calculo da iluminacao w */
    if(ilumCalc){
        changePhong();
    }
    else{ // calculo iluminacao desligado
        changeBasic();
    }
}
function moveBall( variation ){
  //MOVE A BOLA EM FUNCAO DA VARIACAO DE TEMPO
    posX =  Math.abs(30 * (Math.sin(meshBall.userData.step)));
    posZ =  15 * (Math.cos(meshBall.userData.step));
    posZ -= 15;
    meshBall.userData.step += variation ;
    meshBall.position.set(posX, posY, posZ);
}
function moveFlag( variation){
    flag.rotateY((Math.PI/6) * variation);
  // RODA A BADEIRA SOBRE SI MESMA
}
function pause_screen(){
  // FAZ APARECER A JANELA DE PAUSA
  pauseScreen.visible=true;
  controls.enabled=false;

}
function reset(){
    // ALTERAR A POSICAO DA CAMARA E DA BOLA NAS VARIAVEIS GLOBAIS
    // DE REINICIALIZACAO PARA AS POSICOES INICIAIS
    pause = false;
    posX=pos0X;
    posY=pos0Y;
    posZ = pos0Z;
    meshBall.position.set(posX, posY, posZ);
    meshBall.userData.step = 0;
    scene.remove(flag);
    ball_flag= false;
    createFlag();

    controls.enabled=true;
    controls.reset();
}
function restart(){
    //REMOVE DA CENA A MENSAGEM DE PAUSA
    pauseScreen.visible=false;
    controls.enabled=true;

}

/*======================= LOOP PROGRAMA ========================*/
function render( ) {
    'use strict';
    renderer.render(scene, camera1);
    renderer.clearDepth();
    renderer.render(scene1, camera2);

}
function animate() {
    'use strict';

    var variacao;

    if(pause == false && pause_flag == 0){
      clock1.start();
      pause_flag = 1;
      restart();
      pause_block=false;
    }

    else if(pause == true && pause_flag==1 ){
      clock1.stop();
      pause_flag = 0;
      pause_screen();
      pause_block = true;
    }

    if(pause_block == true && reset_flag==1){
      reset();
      reset_flag=0;

    }
    else if( pause_block == true){
    }
    else{

        variacao = clock1.getDelta(); // tempo que passou desde a ultima chamada

        //Update

        calculateLight();
        switchDirectionalLight(); // apaga/liga luz direcional
        switchPointLight(); // apaga/liga luz pontual


        moveFlag(variacao);
        if( ball_flag == true ){
            moveBall(variacao)
        }

        updateWireframe();


        // caso o calculo da iluminacao esteja ligado


        // required if controls.enableDamping or controls.autoRotate are set to true
        // controls.update();


        //Display
        }
        renderer.clear();
        render(); // atualizacao imagem
        requestAnimationFrame(animate);
}
function init() {
    'use strict'; // nao permite utilizar variaveis nao declaradas

    renderer = new THREE.WebGLRenderer({ // gerador da imagem
        antialias: true // reducao do efeito de serrilhamento
    });
    renderer.autoClear = false;

    clock1 = new THREE.Clock(); // temporizador


    renderer.setSize(window.innerWidth, window.innerHeight); // definicao tamanho janela
    scale_width = window.innerWidth * scale;
   	scale_height = window.innerHeight * scale * ratio;


    document.body.appendChild(renderer.domElement); // adicionar o elemento renderer ao doc HTML
    ilumCalc = 0;
    pause=false;
    pause_flag=0;
    ball_flag=false;
    wf_flag=false;

    createScene(); // criacao cena
    createScene1(); // criar cena de pausa
    createCameras(); // criacao das cameras 1, 2 e 3


    render(); // imagem resultante

    window.addEventListener("keydown", onKeyDown); // espera de eventos
    window.addEventListener("resize", onResize);
}
