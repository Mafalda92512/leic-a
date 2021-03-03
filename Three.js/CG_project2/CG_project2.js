//global THREE, requestAnimationFrame, console

//const THREE = require("./three");

var camera, camera1, camera2, camera3, scene, renderer;

var geometry, material, mesh;

var table, barriers, holes, taco1, taco2, taco3, taco4, taco5, taco6, taco_select, ball, all_balls;

var clock1, timer;

var rot_right, rot_left, unlock_rot;

var acceleration = -3;

var dispara, flag;

/*----------------------SCENE-----------------------------*/
function addTable(obj, x, y, z) { // adiciona o tampo da mesa
    'use strict';
    
    geometry = new THREE.CubeGeometry(60, 2, 40); //60 em x e 40 em z
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);

}
function addHole(obj, x, y, z) { // adiciona cilindros que servem de buracos
    geometry = new THREE.CylinderGeometry(1.5, 1.5, 0); //holes com 1.5 raio
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
    
}
function addBarrier(obj, x, y, z, comp, alt, larg) { // adiciona uma barreira a mesa

    geometry = new THREE.CubeGeometry(comp, alt, larg);
    mesh = new THREE.Mesh(geometry, material);
    mesh.position.set(x, y, z);
    obj.add(mesh);
}
function addWhiteBall( temp_vX, temp_vZ, x, y, z, norm_vX, norm_vZ){ // adiciona a bola que vai ser lancada 
    let ball = new THREE.Object3D();
    //recebe a vel de disparo e as normais
    
    ball.userData = { raio: 1, vX: temp_vX , vY:0, vZ: temp_vZ , aX: norm_vX, aY: 0, aZ: norm_vZ, 
                                ball_col:0 , wall_col:0, hole_col:0, b_c: [] };
    
    material = new THREE.MeshBasicMaterial({ color: 0xf0f0f0, wireframe: false }); // branco
    geometry = new THREE.SphereGeometry(1 , 10, 10);
    mesh = new THREE.Mesh(geometry, material);
    ball.add(mesh);

    ball.position.set(x, y, z);
    ball.add(new THREE.AxesHelper(7));
    if (!all_balls.children[0].children[1].visible) { // verifica se as outras bolas tem os eixos visivel
        ball.children[1].visible = !ball.children[1].visible // caso nao tenham, o da nova bola tambem nao vai ser 
    }

    all_balls.add(ball); // adiciona-se a bola ao grupo de todas as bolas na mesa
    scene.add(all_balls); // para a bola entrar para o grupo

}
function createTable(x, y, z) { 
    'use strict';

    material = new THREE.MeshBasicMaterial({ color: 0x00AA55, wireframe: false }); // verde
    //  construcao table 
    table = new THREE.Object3D();
    addTable(table, 0,0,0);
    table.position.set(x, y, z);
    scene.add(table);

}
function createBarriers(x, y, z) { // cria as barreiras que rodeiam a mesa
    'use strict'
    material = new THREE.MeshBasicMaterial({ color: 0x00ff00, wireframe: false }); // verde
    barriers = new THREE.Object3D();
    addBarrier(barriers, 0, 0, 20.25, 61, 2.5, 0.5);    // alt das barreiras = 2.5
    addBarrier(barriers, 0, 0, -20.25, 61, 2.5, 0.5);
    addBarrier(barriers, 30.25, 0, 0, 0.5, 2.5, 41);
    addBarrier(barriers, -30.25, 0, 0, 0.5, 2.5, 41);
    barriers.position.set(x, y, z);
    scene.add(barriers);
}
function createHoles(x, y, z) { // cria os 6 buracos da mesa
    material = new THREE.MeshBasicMaterial({ color: 0x000000, wireframe: false }); 
    holes = new THREE.Object3D();
    
    addHole(holes, 28.5, 1.05, -18.5);
    addHole(holes, 28.5, 1.05,18.5);
    addHole(holes, -28.5, 1.05, -18.5);
    addHole(holes, -28.5, 1.05, 18.5);
    addHole(holes, 0, 1.05, 18.5)
    addHole(holes, 0, 1.05, -18.5);
    
    holes.position.set(x, y, z);
    scene.add(holes);
}
function createTacos() { // cria e adiciona a cena os 6 tacos
    'use strict';
    var material1 = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true }); // azul
    var material2 = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true }); // azul
    var material3 = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true }); // azul
    var material4 = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true }); // azul
    var material5 = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true }); // azul
    var material6 = new THREE.MeshBasicMaterial({ color: 0x0000ff, wireframe: true }); // azul
    geometry = new THREE.CylinderGeometry(0.25, 0.5, 30);
    var taco_1 = new THREE.Mesh(geometry, material1);
    var taco_2 = new THREE.Mesh(geometry, material2);
    var taco_3 = new THREE.Mesh(geometry, material3);
    var taco_4 = new THREE.Mesh(geometry, material4);
    var taco_5 = new THREE.Mesh(geometry, material5);
    var taco_6 = new THREE.Mesh(geometry, material6);
    taco1 = new THREE.Object3D();
    taco2 = new THREE.Object3D();
    taco3 = new THREE.Object3D();
    taco4 = new THREE.Object3D();
    taco5 = new THREE.Object3D();
    taco6 = new THREE.Object3D();

    taco_1.rotateX(-Math.PI/2);
    taco_1.userData = {angle_lock: 0, angle: 0, x_saida:14   , z_saida:18.8 };
    taco1.add(taco_1);
    taco1.position.set(14, 2, 36);
    scene.add(taco1);

    taco_2.rotateX(-Math.PI/2);
    taco_2.userData = {angle_lock: 0, angle: 0, x_saida:-14   , z_saida:18.8 };
    taco2.add(taco_2);
    taco2.position.set(-14, 2, 36);
    scene.add(taco2);

    taco_3.rotateZ(-Math.PI/2);
    taco_3.userData = {angle_lock: 0, angle: -Math.PI/2, x_saida:  -28.8  , z_saida:0 };
    taco3.add(taco_3);
    taco3.position.set(-46, 2, 0);
    scene.add(taco3);

    taco_4.rotateX(Math.PI/2);
    taco_4.userData = {angle_lock: 0, angle: 0, x_saida:-14  , z_saida:-18.8  };
    taco4.add(taco_4);
    taco4.position.set(-14, 2, -36);
    scene.add(taco4);

    taco_5.rotateX(Math.PI/2);
    taco_5.userData = {angle_lock: 0, angle: 0, x_saida: 14  , z_saida:-18.8 };
    taco5.add(taco_5);
    taco5.position.set(14, 2, -36);
    scene.add(taco5);

    taco_6.rotateZ(Math.PI/2);
    taco_6.userData = {angle_lock: 0, angle: Math.PI/2, x_saida: 28.8  , z_saida:0 };
    taco6.add(taco_6);
    taco6.position.set(46, 2, 0);
    scene.add(taco6);
}
function createRandomRedBalls(){ // cria bolas iniciais
    'use strict';
    all_balls = new THREE.Group ();
    
    for (var i=0; i<16; i++){ // 16 e o numero total de bolas iniciais (vermelhas)
        let ball = new THREE.Object3D();
        let temp_vX = THREE.Math.randFloat(-40, 40);    //vel inicial random
        let temp_vZ = THREE.Math.randFloat(-40, 40);
        let norm_vX;
        let norm_vZ;
        
        //normalizacao dos vetores velocidade
        if(Math.abs(temp_vX)<Math.abs(temp_vZ)){
            norm_vZ = temp_vZ / temp_vZ; // dividimos pela variavel com maior valor absoluto
            if(temp_vZ<0){
                norm_vZ= norm_vZ * -1; // para que norm_vZ tenha o mesmo sinal que temp_vZ
                norm_vX = temp_vX / -temp_vZ;
            }
            else{
                norm_vX = temp_vX / temp_vZ;
            }
        }
        else if(Math.abs(temp_vX)>Math.abs(temp_vZ)){
            norm_vX = temp_vX / temp_vX;
            if(temp_vX<0){
                norm_vX= norm_vX * -1;
                norm_vZ = temp_vZ / -temp_vX;
            }
            else{
                norm_vZ = temp_vZ / temp_vX;
            }
        }
        else{ // caso os dois valores absolutos sejam iguais
            norm_vX = temp_vX / temp_vX;
            if(temp_vX<0){
                norm_vX= norm_vX * -1;
            }
            norm_vZ = temp_vZ / temp_vZ;
            if(temp_vZ<0){
                norm_vZ= norm_vZ * -1;
            }
        }
        norm_vX=norm_vX*acceleration; // calculo da aceleracao (com o sentido oposto ao da velocidade)
        norm_vZ=norm_vZ*acceleration;
        
        ball.userData = { raio: 1, vX: temp_vX , vY:0, vZ: temp_vZ , aX: norm_vX, aY: 0, aZ: norm_vZ,
                            ball_col:0 , wall_col:0, hole_col:0, b_c: [] }; // parametros guardados na bola
        material = new THREE.MeshBasicMaterial({ color: 0xff0000, wireframe: false }); // vermelho
        geometry = new THREE.SphereGeometry(1 , 10, 10);
        mesh = new THREE.Mesh(geometry, material);
        
        ball.add(mesh);
        var x= THREE.Math.randFloat(-28.9, 28.9) ; // posicao random da bola (dentro da mesa)
        var y= 2;    //cimo da mesa y=1 + raio da bola
        var z= THREE.Math.randFloat(-18.9, 18.9) ;
        ball.position.set(x, y, z);
        ball.add(new THREE.AxesHelper(7));
        
        all_balls.add(ball); 
    }
    scene.add(all_balls); // adiciona-se todo o grupo de bolas na mesa
}
function createScene() {
    'use strict';

    scene = new THREE.Scene();
    scene.add(new THREE.AxesHelper(10));
    
    createTable(0, 0, 0);
    createBarriers(0, 2, 0);
    createHoles(0,0,0);
    createTacos();
    createRandomRedBalls();
}

/*---------------------CAMERAS----------------------------*/
function createCamera1() {   //criacao camara ortogonal vista de topo
    'use strict';
    
    camera1 = new THREE.OrthographicCamera(-75, 75, -55, 55, 1, 1000); 
    camera1.position.y = 50;
    camera1.lookAt(scene.position);
}
function createCamera2(){ // criacao camara com vista perspetiva
    'use strict';
    
    camera2 = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 1000);
    camera2.position.x = 50;
    camera2.position.y = 50;
    camera2.position.z = 50;
    camera2.lookAt(scene.position);
}
function createCamera3(){ // criacao camara com vista perspetiva
    'use strict';
    // camara segue a ultima bola a ser adicionada ao grupo all_balls (ultima a ser lancada)
    camera3 = new THREE.PerspectiveCamera(70, window.innerWidth / window.innerHeight, 1, 1000);
    camera3.position.x = all_balls.children[all_balls.children.length-1].position.x;
    camera3.position.y = all_balls.children[all_balls.children.length-1].position.y;
    camera3.position.z = all_balls.children[all_balls.children.length-1].position.z;
    camera3.lookAt(scene.position);
}
function createCameras(){ // cria as camaras 1, 2 e 3
    'use strict'
    createCamera1();
    createCamera2();
    createCamera3();
    camera = camera1; // define a camera inicial como a camera1
}
function changeToCamera1(){
    'use strict'
    camera = camera1;
}
function changeToCamera2(){
    'use strict'
    camera = camera2;
}
function changeToCamera3(){ //camara movel
    'use strict'
    camera=camera3;
}
function atualizaCamera3(){ // atualiza a posicao da camara3 de acordo com a posicao da ultima bola a ser lancada
    
    //calcula normal de acordo com a vel da ultima bola a ser criada
    let norm = new THREE.Vector3(all_balls.children[all_balls.children.length-1].userData.vX,
                                    all_balls.children[all_balls.children.length-1].userData.vY, 
                                        all_balls.children[all_balls.children.length-1].userData.vZ );
    norm.normalize();
    // atualiza posicao da camara para que esta siga o movimento da bola a alguma distancia 
    camera3.position.x = all_balls.children[all_balls.children.length-1].position.x - norm.x*3 ;
    camera3.position.y = all_balls.children[all_balls.children.length-1].position.y - norm.y*3 + 20;
    camera3.position.z = all_balls.children[all_balls.children.length-1].position.z - norm.z*3;

    camera3.lookAt(all_balls.children[all_balls.children.length-1].position);
}

/*----------------VERIFICACAO DE COLISOES------------------*/
function hasWallColision(ball){ // verifica se a bola colidiu com uma das barreiras
        let iX=ball.position.x;
        let iZ=ball.position.z;
        
        let i=0;
            if (iX >= 29 || iX <= -29) { // colidiu com uma das laterais
                
                ball.userData.wall_col=1;
                return true
            }
            if (iZ >= 19 || iZ <= -19) { 
                
                ball.userData.wall_col=2;
                return true
            }
            if( (iZ >= 19 || iZ <= -19) && (iX >= 29 || iX <= -29) ){ // colidiu com um canto, nao acontece devido aos buracos 
                ball.userData.wall_col=3;
                return true
            }
    return false

}
function hasBallColision(ball , i){ // verifica se uma bola colidiu com outra
    let n = all_balls.children.length;
    let iX = ball.position.x;
    let iY = ball.position.y;
    let iZ = ball.position.z;
    for( let j=i+1 ; j < n ; j++ ){ // verifica se uma bola (i) interseta qualquer outra bola de indice superior (j) no grupo all_balls 
        let jX=all_balls.children[j].position.x;
        let jZ=all_balls.children[j].position.z;
        let jY=all_balls.children[j].position.y;
        if( 4 >= ( (iX-jX)*(iX-jX) )+ ((iZ-jZ)*(iZ-jZ) ) && jY == 2 && iY == 2){ // apenas prossegue se nenhuma das bolas estiver a cair e se ambas se intersetarem
            ball.userData.ball_col += 1; // guarda o numero de colisoes de uma bola
            ball.userData.b_c.push(all_balls.children[j]); // guarda o indice da bola com que colidiu
            return true;
        }
    }
    return false;  
}
function hasHoleColision(ball){ // verifica se a bola caiu num buraco
    let posX=ball.position.x;
    let posZ=ball.position.z;
    
    for(let i=0; i<6 ; i++){ // percorre todos os buracos
        // se o centro da bola esta a um raio < 1 do centro do buraco e nao estiver ja a cair (y!=2)
        if( (holes.children[i].position.x-1 < posX) && (posX < holes.children[i].position.x+1) && 
        (holes.children[i].position.z-1< posZ) && (posZ < holes.children[i].position.z+1 ) && (ball.position.y == 2)){ 
            ball.userData.hole_col=1;
        }
    }

}

/*----------------TRATAMENTO DE COLISOES------------------*/
function WallColision(ball, variacao ){ // trata da colisao bola - parede
    let aux_temp= variacao/10; 
    
    if (ball.userData.wall_col == 1){ // se a colisao foi verificada (contra uma das paredes laterais)
        while(ball.position.x >= 29 || ball.position.x <= -29){// enquanto a bola estiver a intersetar a parede
            ball.position.x -= ball.userData.vX * aux_temp; // posicao da bola recua aux_temp no tempo
            aux_temp += aux_temp;
        }
               
        ball.userData.vX = ball.userData.vX * -1; // troca de sentidos da velocidade e aceleracao
        ball.userData.aX = ball.userData.aX * -1;

        ball.userData.vX = ball.userData.vX + ball.userData.aX * aux_temp; // calculo das novas velocidades (em funcao do tempo recuado)
        ball.userData.vZ = ball.userData.vZ + ball.userData.aZ * aux_temp;

        // calculo da nova posicao da bola, contando com o movimento realizado no tempo recuado
        ball.position.x +=
                ball.userData.vX*aux_temp + (ball.userData.aX * aux_temp * aux_temp) / 2;
        ball.position.z +=
                ball.userData.vZ*aux_temp + (ball.userData.aZ * aux_temp * aux_temp ) / 2;  
    }

    else if (ball.userData.wall_col == 2){ // se a colisao foi verificada (parede de frente ou de tras)
        while(ball.position.z >= 19 || ball.position.z <= -19){
            ball.position.z -= ball.userData.vZ*aux_temp;
            aux_temp += aux_temp;
        }
        ball.userData.extra= aux_temp;
        ball.userData.vZ = ball.userData.vZ * -1;
        ball.userData.aZ = ball.userData.aZ * -1;
        
        ball.userData.vX = ball.userData.vX + ball.userData.aX * aux_temp;
        ball.userData.vZ = ball.userData.vZ + ball.userData.aZ * aux_temp;

        ball.position.x +=
                ball.userData.vX*aux_temp + (ball.userData.aX * aux_temp * aux_temp) / 2;
        ball.position.z +=
                ball.userData.vZ*aux_temp + (ball.userData.aZ * aux_temp * aux_temp ) / 2;
    }

    else if (ball.userData.wall_col == 3){ // se a colisao foi verificada contra um canto - nao ocorre devido aos buracos
        while((iZ >= 19 || iZ <= -19) && (iX >= 29 || iX <= -29)){
            ball.position.x -= ball.userData.vX*aux_temp;
            ball.position.z -= ball.userData.vZ*aux_temp;
            aux_temp += aux_temp;
        }
        ball.userData.extra= aux_temp;
        ball.userData.vZ = ball.userData.vZ * -1;
        ball.userData.aZ = ball.userData.aZ * -1;
        ball.userData.vZ = ball.userData.vX * -1;
        ball.userData.aZ = ball.userData.aX * -1;

    }
    ball.userData.wall_col=0; // flag da colisao volta a 0 depois de esta ser tratada
}
function BallColision(ball1 , time){ // trata da colisao bola - bola
    let i = ball1.userData.ball_col; // numero de colisoes
    for(i; 0 < i; i--){
        aux_temp = time/12;
        ball2= ball1.userData.b_c[i-1]; // ultimo elemento da lista
        iX = ball1.position.x;
        iZ = ball1.position.z;
        jX = ball2.position.x;
        jZ = ball2.position.z;
        
        while(4 >= ( (iX-jX)*(iX-jX) )+ ((iZ-jZ)*(iZ-jZ) ) ){ // enquanto as bolas se intersetem
            iX -= ball1.userData.vX * aux_temp; // calculo de uma posicao das bolas, recuadas aux_temp no tempo
            iZ -= ball1.userData.vZ * aux_temp;
            jX -= ball2.userData.vX * aux_temp;
            jZ -= ball2.userData.vZ * aux_temp;
            aux_temp += aux_temp;
        }
        
        ball1.position.x = iX; // atualizacao da posicao das bolas para que estas nao se intersetem
        ball1.position.z = iZ;
        ball2.position.x = jX;
        ball2.position.z = jZ;
        
        BallsColide(ball1, ball2 , aux_temp);
        ball1.userData.b_c.pop(); 
        ball1.userData.ball_col-= 1;
    }
}
function BallsColide( ball1 , ball2, time){ // tratamento das velocidades e posicoes das bolas apos uma colisao
    
        let b1_vec_vel = new THREE.Vector3(ball1.userData.vX, ball1.userData.vY, ball1.userData.vZ);
        let b2_vec_vel = new THREE.Vector3(ball2.userData.vX, ball2.userData.vY, ball2.userData.vZ);
        let b1_vec_pos = ball1.position.clone();
        let b2_vec_pos = ball2.position.clone();
        let b1_vf =  b1_vec_vel.clone();
        let b2_vf =  b2_vec_vel.clone();
        let b1_dup_vel = b1_vec_vel.clone();
       
        let b1_dup_pos = b1_vec_pos.clone();
        let b2_dup_pos = b2_vec_pos.clone();
   
        // colisao elastica
        let aux1 = ( (b1_vec_vel.sub(b2_vec_vel)).dot(b1_vec_pos.sub(b2_vec_pos) ) )/ (Math.pow(b1_dup_pos.distanceTo(b2_vec_pos), 2));
        b1_vf.sub((b1_dup_pos.sub(b2_vec_pos)).multiplyScalar(aux1) );
    
        let aux2 = ( (b2_vec_vel.sub(b1_dup_vel)).dot(b2_vec_pos.sub(ball1.position) ) )/ (Math.pow(b2_dup_pos.distanceTo(ball1.position), 2));
        b2_vf.sub((b2_dup_pos.sub(ball1.position)).multiplyScalar(aux2) ) ;

        // novas velocidades
        ball1.userData.vX = b1_vf.x;
        ball1.userData.vY = 0;
        ball1.userData.vZ = b1_vf.z;
        ball2.userData.vX = b2_vf.x;
        ball2.userData.vY = 0;
        ball2.userData.vZ = b2_vf.z;
    
        b1_vf.normalize();
        b2_vf.normalize();
        // novas aceleracoes
        ball1.userData.aX = b1_vf.x*acceleration;
        ball1.userData.aY = 0;
        ball1.userData.aZ = b1_vf.z*acceleration;
        ball2.userData.aX = b2_vf.x*acceleration;
        ball2.userData.aY = 0;
        ball2.userData.aZ = b2_vf.z*acceleration;
        
        // velocidade das bolas a contar com o tempo recuado
        ball1.userData.vX = ball1.userData.vX + ball1.userData.aX * time;
        ball2.userData.vX = ball2.userData.vX + ball2.userData.aX * time;
        ball1.userData.vZ = ball1.userData.vZ + ball1.userData.aZ * time;
        ball2.userData.vZ = ball2.userData.vZ + ball2.userData.aZ * time;

        // posicao das bolas a contar com o tempo recuado
        ball1.position.x +=
                ball1.userData.vX*time + (ball1.userData.aX * time * time) / 2;
        ball1.position.z +=
                ball1.userData.vZ*time + (ball1.userData.aZ * time * time ) / 2;
        ball2.position.x +=
                ball2.userData.vX*time + (ball2.userData.aX * time * time) / 2;
        ball2.position.z +=
                ball2.userData.vZ*time+ (ball2.userData.aZ * time * time ) / 2;
}
function HoleColision(ball){ // trata da colisao bola - buraco
    if(ball.userData.hole_col == 1){ // se a flag desta colisao estiver a 1
        ball.userData.vX = 0;
        ball.userData.vZ = 0;
        ball.userData.vY = -5; // velocidade com que a bola vai cair
        ball.position.y -= 0.1; // para nao se considerarem mais nenhumas colisoes com esta bola
    }
    ball.userData.hole_col = 0; // apos o tratamento da colisao, a flag volta a 0

}

/*----------------------EVENTOS-----------------------*/
function onResize() {    //atualiza o tamanho da janela
    'use strict';
   
    renderer.setSize(window.innerWidth, window.innerHeight);

    if (window.innerHeight > 0 && window.innerWidth > 0) {
        camera.aspect = window.innerWidth / window.innerHeight;
        camera.updateProjectionMatrix();
    }

}
function fireBall(){ // funcao que lanca uma bola (branca)
    let x= taco_select.children[0].userData.x_saida; // parametros de saida da bola
    let z= taco_select.children[0].userData.z_saida;
    let angle = taco_select.children[0].userData.angle;
    vel = 40;
    let vel_x;
    let vel_z;
    let a_x;
    let a_z;
    
    vel_x = Math.sin(angle)*vel; // calculo da velocidade e aceleracao iniciais da bola
    vel_z = Math.cos(angle)*vel;
    a_x = Math.sin(angle)*acceleration;
    a_z = Math.cos(angle)*acceleration;
    
    addWhiteBall(vel_x, vel_z, x , 2, z, a_x, a_z); // adiciona a bola branca com os parametros acima indicados

    dispara=false; // bola ja foi disparada
}
function onKeyDown(e) {
    'use strict';

    switch (e.keyCode) {
    
    case 52: // tecla 4     taco1 selecionado
        unlock_rot = 1; // apos se pressionar uma das teclas para selecionar um taco, esse taco ja pode ser movido/rodado
        taco_select.children[0].material.color.setHex(0x0000ff); // taco selecionado anteriormente volta a cor original
        taco_select = taco1;
        taco_select.children[0].material.color.setHex(0xff0000); // taco selecionado passa a vermelho
        break;
    case 53: // tecla 5     taco2 selecionado
        unlock_rot = 1;
        taco_select.children[0].material.color.setHex(0x0000ff);
        taco_select = taco2;
        taco_select.children[0].material.color.setHex(0xff0000);
        break;
    case 54: // tecla 6     taco3 selecionado
        unlock_rot = 1;
        taco_select.children[0].material.color.setHex(0x0000ff);
        taco_select = taco3;
        taco_select.children[0].material.color.setHex(0xff0000);
        break;
    case 55: // tecla 7     taco4 selecionado
        unlock_rot = 1;
        taco_select.children[0].material.color.setHex(0x0000ff);
        taco_select = taco4;
        taco_select.children[0].material.color.setHex(0xff0000);
        break;
    case 56: // tecla 8     taco5 selecionado
        unlock_rot = 1;
        taco_select.children[0].material.color.setHex(0x0000ff);
        taco_select = taco5;
        taco_select.children[0].material.color.setHex(0xff0000);
        break;
    case 57: // tecla 9     taco6 selecionado
        unlock_rot = 1;
        taco_select.children[0].material.color.setHex(0x0000ff);
        taco_select = taco6;
        taco_select.children[0].material.color.setHex(0xff0000);
        break;
    case 69:  //E
    case 101: //e
        let n = all_balls.children.length;
        for (var i=0; i < n; i++) {
            all_balls.children[i].children[1].visible = !all_balls.children[i].children[1].visible; // eixos das bolas passam ou n a ser visiveis
        }
        scene.children[0].visible = !scene.children[0].visible; // eixo do mundo passa ou n a ser visivel
        break;
    case 49: //tecla 1 mudar a camara para projecao ortogonal vista topo
        changeToCamera1();
        break;
    case 50: //tecla 2 mudar a camara para perspetiva
        changeToCamera2();
        break;
    case 51: //tecla 3 mudar a camara para movel
        changeToCamera3();
        break;
    case 37: // seta para esquerda
        if (unlock_rot == 1) {
           rot_left=1; 
        }
        break;
    case 39: // seta para direita 
        if (unlock_rot == 1) {
            rot_right=1; 
        }
        break;
    case 32:
        if(unlock_rot==1){ // apenas e possivel disparar uma bola caso um taco ja tenha sido selecionado
            dispara = true;
        }
    }

}
function onKeyUp(e) {
    'use strict';

    switch (e.keyCode) {
    
    case 37: // seta para esquerda
        rot_left=0;
        break;

    case 39: // seta para direita 
        rot_right=0;
        break;
    }
}
function moveTaco(taco) {
    'use strict'

    switch(taco) {
        case taco1:
        case taco2:
            if ((rot_right==1) && (taco.children[0].userData.angle_lock > -4)) { // se ainda puder rodar
                taco.rotation.y -= 0.01;
                taco.children[0].userData.angle += 0.01;
                // translacao do taco para a ponta permanecer no mesmo sitio
                taco.position.x = taco.children[0].userData.x_saida -15*Math.cos(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida+2.2+ 15*Math.sin(Math.PI/2-taco.rotation.y);
                taco.children[0].userData.angle_lock -= 0.05;
            }
            if((rot_left==1) && (taco.children[0].userData.angle_lock < 4)){
                taco.rotation.y += 0.01;
                taco.children[0].userData.angle -= 0.01;
                taco.position.x = taco.children[0].userData.x_saida - 15 *Math.cos(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida +2.2+15*Math.sin(Math.PI/2-taco.rotation.y);
                taco.children[0].userData.angle_lock += 0.05;
            }
            break;
        case taco4:
        case taco5:
            if ((rot_right==1) && (taco.children[0].userData.angle_lock > -4)) { // se ainda puder rodar
                taco.rotation.y -= 0.01;
                taco.children[0].userData.angle -= 0.01;
                taco.position.x = taco.children[0].userData.x_saida +15*Math.cos(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida-2.2- 15*Math.sin(Math.PI/2-taco.rotation.y);
                taco.children[0].userData.angle_lock -= 0.05;
            }
            if((rot_left==1) && (taco.children[0].userData.angle_lock < 4)){
                taco.rotation.y += 0.01;
                taco.children[0].userData.angle += 0.01;
                taco.position.x = taco.children[0].userData.x_saida + 15 *Math.cos(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida -2.2-15*Math.sin(Math.PI/2-taco.rotation.y);
                taco.children[0].userData.angle_lock += 0.05;
            }   
            break;
        case taco3:
            if ((rot_right==1) && (taco.children[0].userData.angle_lock > -4)) { // se ainda puder rodar

                taco.rotation.y -= 0.01;
                taco.children[0].userData.angle += 0.01 ;
                taco.position.x = taco.children[0].userData.x_saida -2.2- 15*Math.sin(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida +15*Math.cos(Math.PI/2-taco.rotation.y);
                taco.children[0].userData.angle_lock -= 0.05;
            }
            if((rot_left==1) && (taco.children[0].userData.angle_lock < 4)){

                taco.rotation.y += 0.01;
                taco.children[0].userData.angle -= 0.01 ;
                taco.position.x = taco.children[0].userData.x_saida -2.2- 15*Math.sin(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida +15*Math.cos(Math.PI/2-taco.rotation.y);
                taco.children[0].userData.angle_lock += 0.05;
            }
            break;
        case taco6:
            if ((rot_right==1) && (taco.children[0].userData.angle_lock > -4)) { // se ainda puder rodar

                taco.rotation.y -= 0.01;
                taco.children[0].userData.angle += 0.01 ;
                taco.position.x = taco.children[0].userData.x_saida +2.2+ 15*Math.sin(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida +15*Math.cos(Math.PI/2+taco.rotation.y);
                taco.children[0].userData.angle_lock -= 0.05;

            }
            if((rot_left==1) && (taco.children[0].userData.angle_lock < 4)){

                taco.rotation.y += 0.01;
                taco.children[0].userData.angle -= 0.01 ;
                taco.position.x = taco.children[0].userData.x_saida +2.2+ 15*Math.sin(Math.PI/2+taco.rotation.y);
                taco.position.z = taco.children[0].userData.z_saida +15*Math.cos(Math.PI/2+taco.rotation.y);
                taco.children[0].userData.angle_lock += 0.05;
            }
            break;
    }

}
function updateBalls(variacaoTempo) {
    time = variacaoTempo;
    let n = all_balls.children.length ;

    for (var i=0; i < n; i++) { // percorre todas as bolas do grupo all_balls
         
        vx=all_balls.children[i].userData.vX;
        vy=all_balls.children[i].userData.vY;
        vz=all_balls.children[i].userData.vZ;
        
        if( vx*all_balls.children[i].userData.aX < 0 || vz*all_balls.children[i].userData.aZ < 0 ){ // caso a velocidade e a aceleracao no mesmo eixo tenham sinais contrarios 
            all_balls.children[i].userData.vX = vx + all_balls.children[i].userData.aX * time; // atualiza velocidade (movimento uniformemente retardado)
            all_balls.children[i].userData.vZ = vz + all_balls.children[i].userData.aZ * time;
            
            all_balls.children[i].position.x += // atualiza posicao da bola (movimento uniformemente retardado)
                    vx*time + (all_balls.children[i].userData.aX * time * time ) / 2;
            all_balls.children[i].position.z +=
                    vz*time + (all_balls.children[i].userData.aZ * time * time ) / 2;    
            rotateBall(all_balls.children[i], time);
        }
        else if (vy != 0) { // caso a bola esteja a cair
            all_balls.children[i].userData.vX = 0;
            all_balls.children[i].userData.vZ = 0;
            all_balls.children[i].userData.vY = vy + all_balls.children[i].userData.aY * time; 
            // variam apenas a sua velocidade e posicao em y
            all_balls.children[i].position.y +=
                    vy*time + (all_balls.children[i].userData.aY * time * time ) / 2;
        }
        else{ // caso contrario a bola para
            all_balls.children[i].userData.vX = 0;
            all_balls.children[i].userData.vZ = 0;
        }
    }
}
function rotateBall(ball, time){ // trata da rotacao das bolas em funcao de uma variacao de tempo
    let eixo_x = new THREE.Vector3(0, 0, 1);
    let eixo_z = new THREE.Vector3(1, 0, 0);
    ball.rotateOnWorldAxis(eixo_x , -(((ball.userData.vX * time *time)/2)/(ball.userData.raio* Math.PI / 180)));
    ball.rotateOnWorldAxis(eixo_z , (((ball.userData.vZ * time *time)/2)/(ball.userData.raio* Math.PI / 180 )));
}
    
//_______________________________________________________________________
function render() { 
    'use strict';
    
    renderer.render(scene, camera);
}
function animate() {
    'use strict';
    
    var variacao;
    variacao = clock1.getDelta(); // tempo que passou desde a ultima chamada
    if(dispara){ // se foi premido o botao para lancar uma bola
        fireBall();
    }
    moveTaco(taco_select, variacao);
    updateBalls(variacao);
    atualizaCamera3();

    for (let i=0; i < all_balls.children.length; i++){ // verificacao de colisoes
        hasWallColision(all_balls.children[i] );
        hasBallColision(all_balls.children[i], i);  
        hasHoleColision(all_balls.children[i])
    }
    for (let i=0; i < all_balls.children.length; i++){ // tratamento de colisoes
        WallColision(all_balls.children[i] , variacao);
        BallColision(all_balls.children[i] , variacao);
        HoleColision(all_balls.children[i]);
    }
    
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
    document.body.appendChild(renderer.domElement); // adicionar o elemento renderer ao doc HTML

    createScene(); // criacao cena
    createCameras(); // criacao das cameras 1, 2 e 3
    taco_select = taco1; // taco selecionado comeca por ser o taco1
    unlock_rot = 0; 
    render(); // imagem resultante

    window.addEventListener("keydown", onKeyDown); // espera de eventos
    window.addEventListener("keyup", onKeyUp);
    window.addEventListener("resize", onResize);
} 
