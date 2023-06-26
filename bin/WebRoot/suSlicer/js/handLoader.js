if ( ! Detector.webgl ) Detector.addGetWebGLMessage();

var camera, scene, renderer;
var edge_width = 20

init();

function init() {

  scene = new THREE.Scene();
  scene.add( new THREE.AmbientLight( 0x999999 ) );

  camera = new THREE.PerspectiveCamera( 35, window.innerWidth / window.innerHeight, 1, 500 );

  // Z is up for objects intended to be 3D printed.

  camera.up.set( 0, 0, 1 );
  camera.position.set( 0, -9, 6 );

  camera.add( new THREE.PointLight( 0xffffff, 0.8 ) );

  scene.add( camera );

  var grid = new THREE.GridHelper( 25, 50, 0xffffff, 0x555555 );
  grid.rotateOnAxis( new THREE.Vector3( 1, 0, 0 ), 90 * ( Math.PI/180 ) );
  scene.add( grid );

  renderer = new THREE.WebGLRenderer( { antialias: true } );
  renderer.setClearColor( 0x999999 );
  renderer.setPixelRatio( window.devicePixelRatio );

  Viewer3D = document.getElementById('ViewerSTL');
  rect = Viewer3D.getBoundingClientRect();

  renderer.setSize( rect.width-edge_width, rect.height-edge_width);
  Viewer3D.appendChild(renderer.domElement);

  var loader = new THREE.STLLoader();


  // Binary files
  var material = new THREE.MeshPhongMaterial( { color: 0xffb755, specular: 0x111111, shininess: 200 } );
  loader.load( 'models/bunny.stl', function ( geometry ) {
    var mesh = new THREE.Mesh( geometry, material );
    mesh.position.set( 0, 0, 0 );
    mesh.rotation.set( 0, 0, 0 );
    mesh.scale.set( 1, 1, 1 );

    mesh.castShadow = true;
    mesh.receiveShadow = true;

    scene.add( mesh );
    render();
  });

  var controls = new THREE.OrbitControls( camera, renderer.domElement );
  controls.addEventListener( 'change', render );
  controls.target.set( 0, 1.2, 2 );
  controls.update();
  window.addEventListener( 'resize', onWindowResize, false );

}

function onWindowResize() {
  edge_width = 20
  Viewer3D = document.getElementById('ViewerSTL');
  rect = Viewer3D.getBoundingClientRect();
  camera.aspect = rect.width / rect.height;
  camera.updateProjectionMatrix();

  renderer.setSize( rect.width - edge_width, rect.height - edge_width );
  render();
}

function render() {

  renderer.render( scene, camera );

}
