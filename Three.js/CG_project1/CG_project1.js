/*global THREE, requestAnimationFrame, console*/

var camera, scene, renderer;

var geometry, material, mesh;

var mobile, g2, g1;

var clock;

var v1d, v1e, v2d, v2e, v3d, v3e;
var direita, esquerda, frente, tras;

function addCube(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CubeGeometry(1, 1, 1);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}
function addCubeLong(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CubeGeometry(1, 2, 1);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}
function addCylinder(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CylinderGeometry(0.2, 0.2, 5);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}

function addCylinderTip(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CylinderGeometry(1, 1, 1);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}

function addCylinderHolder(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CylinderGeometry(0.2, 0.2, 2);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    mesh.rotateZ(Math.PI/2);
    obj.add(mesh);

}

function addCylinder90(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CylinderGeometry(0.2, 0.2, 5);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    mesh.rotateX(Math.PI/2);
    obj.add(mesh);

}
function addCylinderShort(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CylinderGeometry(0.2, 0.2, 3);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}

function addCubeHolder(obj, x, y, z) {
    'use strict';

    geometry = new THREE.CubeGeometry(0.2, 1, 0.2);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}

function createMobile(x, y, z) {
    'use strict';

    material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: true }); // verde
    
  //  construcao mobile 

  //v3 para baixo
  g2 = new THREE.Object3D();
  
  addCylinderShort(g2, x, y-14, z);
  addCylinder90(g2, x, y-15.5, z);
  addCylinderTip(g2, x, y-15.5, z-3.5);  
  addCylinderHolder(g2, x-1, y-15.5, z);   
  addCylinderTip(g2, x-3, y-15.5, z);
  addCylinderShort(g2, x, y-17, z+2.5);
  addCylinder90(g2, x, y-18.5, z+5);
  addCylinder90(g2, x, y-18.5, z);
  addCylinderTip(g2, x, y-18.5, z-3.5);
  addCubeHolder(g2, x, y-19, z+5);
  addCube(g2, x, y-20, z+5);
  addCube(g2, x, y-18.5, z+8);
  g2.position.set(x, y-12, z+5);

  // v2 para baixo
  g1 = new THREE.Object3D();

  addCylinder(g1, x, y-10, z);
  addCylinder90(g1, x, y-12.5, z-2.5);
  addCylinder90(g1, x, y-12.5, z+2.5);
  addCylinderShort(g1, x, y-14, z-5);
  addCylinder90(g1, x, y-15.5, z-7.5);
  addCubeHolder(g1, x, y-16, z-6);
  addCube(g1, x, y-17, z-6 );
  addCubeHolder(g1, x, y-16, z-8);
  addCube(g1, x, y-17, z-8);
  addCylinderTip(g1, x, y-15.5, z-11);

  g1.add(g2);
  g1.position.set(x, y-12 , z-9);

  //v1 para baixo, mobile inteiro
  mobile = new THREE.Object3D();

  addCylinder(mobile, x, y, z);
  addCylinder90(mobile, x, y-2.5, z+2.5);
  addCylinder90(mobile, x, y-2.5, z-2.5);
  addCylinder(mobile, x, y-5, z-5);
  addCylinder(mobile, x, y-5, z+5);
  addCylinder90(mobile, x, y-7.5, z+7.5);
  addCube(mobile, x, y-7.5, z+10.5);
  addCubeHolder(mobile, x, y-8, z+7.5);
  addCubeLong(mobile, x, y-9.5, z+7.5);
  addCubeHolder(mobile, x, y-8, z+5);
  addCubeLong(mobile, x, y-9.5, z+5);
  addCylinder90(mobile, x, y-7.5, z-7.5);
  addCylinderTip(mobile, x, y-7.5, z-11);
  

  mobile.add(g1);
  mobile.position.set(x, y, z);
  scene.add(mobile);

}

function createScene() {
    'use strict';

    scene = new THREE.Scene();
    scene.add(new THREE.AxisHelper(10));
    
    createMobile(0, 12, 0);
}

function createCamera() {   //criacao camara ortogonal vista frontal
    'use strict';
    
    camera = new THREE.OrthographicCamera(-30, 30, -30, 30, 1, 1000); 
    camera.position.x = 50;
    
    camera.lookAt(scene.position);
    camera.rotation.z=Math.PI;
}

function createCamera1(){ // vista frontal eixo x
    'use strict';
    
    camera.position.x = 50;
    camera.position.y = 0;
    camera.position.z = 0;
    camera.lookAt(scene.position);
    camera.rotation.z=Math.PI;
}


function createCamera2(){ // vista topo eixo y
    'use strict';
    
    camera.position.x = 0;
    camera.position.y = 50;
    camera.position.z = 0;
    camera.lookAt(scene.position);
    
}

function createCamera3(){ // vista lateral eixo z
    'use strict';
    
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 50;
    camera.lookAt(scene.position);
    camera.rotation.z=Math.PI; // rotacao 180
    
}

function onResize() {    //atualiza o tamanho da janela
    'use strict';
   
    renderer.setSize(window.innerWidth, window.innerHeight);

    if (window.innerHeight > 0 && window.innerWidth > 0) {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
    }

}

function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
    
    case 52: // tecla 4     troca entre modelo de arames e solido
        scene.traverse(function (node) {
            if (node instanceof THREE.Mesh) {
                node.material.wireframe = !node.material.wireframe;
            }
        });
        break;
    case 69:  //E
    case 101: //e
        scene.traverse(function (node) {
            if (node instanceof THREE.AxisHelper) {
                node.visible = !node.visible;
            }
        });
        break;

    case 49: //tecla 1 mudar a camara para projecao ortogonal vista frontal
        createCamera1();
        break;
    case 50: //tecla 2 mudar a camara para projecao ortogonal vista de topo
        createCamera2();
        break;
    case 51: //tecla 3 mudar a camara para projecao ortogonal vista lateral
        createCamera3();
        break;
    case 81: //Q
    case 113: //q angulo v1 direcao ponteiro dos relogios
        v1d=1;
        break;
    case 87: //W
    case 119://w angulo v1 direcao contraria ponteiro dos relogios
        v1e=1;
        break;
    case 65: //A
    case 97: //a angulo v2 direcao ponteiro dos relogios
        v2d=1;
        break;
    case 68: //D
    case 100://d angulo v2 direcao contraria ponteiro dos relogios
        v2e=1;
        break;
    case 90: //Z
    case 122: //z angulo v3 direcao ponteiro dos relogios
        v3d=1;
        break;
    case 67: //C
    case 99://c angulo v3 direcao contraria ponteiro dos relogios
        v3e=1;
        break;
    case 37: // seta para esquerda
        esquerda=1;
        break;
    case 38: // seta para frente 
        frente=1;
        break;
    case 39: // seta para direita 
        direita=1;
        break;
    case 40: // seta para tras
        tras=1;
        break;
    }
}

function onKeyUp(e) {
    'use strict';

    switch (e.keyCode) {
    

    case 81: //Q
    case 113: //q angulo v1 direcao ponteiro dos relogios
        v1d=0;
        break;
    case 87: //W
    case 119://w angulo v1 direcao contraria ponteiro dos relogios
        v1e=0;
        break;
    case 65: //A
    case 97: //a angulo v2 direcao ponteiro dos relogios
        v2d=0;
        break;
    case 68: //D
    case 100://d angulo v2 direcao contraria ponteiro dos relogios
        v2e=0;
        break;
    case 90: //Z
    case 122: //z angulo v3 direcao ponteiro dos relogios
        v3d=0;
        break;
    case 67: //C
    case 99://c angulo v3 direcao contraria ponteiro dos relogios
        v3e=0;
        break;
    case 37: // seta para esquerda
        esquerda=0;
        break;
    case 38: // seta para frente 
        frente=0;
        break;
    case 39: // seta para direita 
        direita=0;
        break;
    case 40: // seta para tras 
        tras=0;
        break;
    }
}

function rotateMobile(mobile, variacao) { // movimentos Q, W, A, D, Z, C

    if(v1d==1){ // se tiver sido premido
        mobile.rotateY(1  * variacao); // altera posicao mobile
    }
    if(v1e==1){
        mobile.rotateY (-1  * variacao);
    }
    if(v2d==1){
        g1.rotateY(1  * variacao); // altera posicao g1
    }
    if(v2e==1){
        g1.rotateY(-1 * variacao);
    }
    if(v3d==1){
        g2.rotateY(1  * variacao); // altera posicao g2
    }
    if(v3e==1){
        g2.rotateY(-1  * variacao);
    }
}

function moveMobile(mobile , variacao ){ // movimentos setas

    if(direita==1) { // se tiver sido premido
        mobile.position.x +=  1 * variacao ; // altera posicao mobile inteiro
    }
    if(esquerda==1) {
        mobile.position.x -= 1* variacao;
    }
    if(frente==1) {
        mobile.position.z += 1* variacao;
    }
    if(tras==1) {
        mobile.position.z -= 1* variacao;
    }
    
} 

function render() { 
    'use strict';
    
    renderer.render(scene, camera);
}

function animate() {
    'use strict';

    var variacao;
    variacao = clock.getDelta(); // tempo que passou desde a ultima chamada

    rotateMobile(mobile, variacao); // calculo novas posicoes objectos
    moveMobile(mobile, variacao);

    render(); // atualizacao imagem
    requestAnimationFrame(animate);     

}

function init() {
    'use strict'; // nao permite utilizar variaveis nao declaradas
    
    renderer = new THREE.WebGLRenderer({ // gerador da imagem
        antialias: true // reducao do efeito de serrilhamento
    });
    
    clock = new THREE.Clock(); // temporizador
    clock.start();
  
    renderer.setSize(window.innerWidth, window.innerHeight); // definicao tamanho janela
    document.body.appendChild(renderer.domElement); // adicionar o elemento renderer ao doc HTML

    createScene(); // criacao cena
    createCamera(); // criacao da camara inicial (1)

    render(); // imagem resultante

    window.addEventListener("keydown", onKeyDown); // espera de eventos
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);
} 