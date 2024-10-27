import * as THREE from 'three';
import { OrbitControls } from 'three/examples/jsm/controls/OrbitControls';
import * as CANNON from './js-libs/cannon-es.js';

// Scene setup
const scene = new THREE.Scene();
const camera = new THREE.PerspectiveCamera(75, window.innerWidth / window.innerHeight, 0.1, 1000);
const renderer = new THREE.WebGLRenderer();
renderer.setSize(window.innerWidth, window.innerHeight);
renderer.shadowMap.enabled = true;
document.body.appendChild(renderer.domElement);

// OrbitControls
const controls = new OrbitControls(camera, renderer.domElement);
controls.enableDamping = true;
controls.dampingFactor = 0.25;

// Lighting
// const cameraLight = new THREE.SpotLight(0xff0000, 1);
// cameraLight.position.set(-5, 5, 5).normalize();
// cameraLight.castShadow = true;
// scene.add(cameraLight);

const light = new THREE.DirectionalLight(0xffffff, 1);
light.position.set(5, 5, 5).normalize();
light.castShadow = true;
// light.shadowDarkness = 0.9;
// light.shadowCameraVisible = true; // only for debugging
scene.add(light);

const toplight = new THREE.DirectionalLight(0xffffff, 0.2);
toplight.position.set(-5, 10, -5).normalize();
toplight.castShadow = true;
// light.shadowDarkness = 0.9;
// toplight.shadowCameraVisible = true; // only for debugging
scene.add(toplight);

// Cube
const loader = new THREE.TextureLoader();
const textures = [
    loader.load('box.jpg'),
    loader.load('box.jpg'),
    loader.load('box.jpg'),
    loader.load('box.jpg'),
    loader.load('box.jpg'),
    loader.load('box.jpg')
];
const cubeMaterial = textures.map(texture => new THREE.MeshStandardMaterial({ map: texture }));

const geometry = new THREE.BoxGeometry();
// const cubeMaterial = new THREE.MeshStandardMaterial({ color: 0x0077ff });
const cube = new THREE.Mesh(geometry, cubeMaterial);
cube.castShadow = true;
cube.receiveShadow = true;
scene.add(cube);

// Ground
const groundGeometry = new THREE.PlaneGeometry(10, 10);
const groundMaterial = new THREE.MeshStandardMaterial({ color: 0x10ff10 });
const ground = new THREE.Mesh(groundGeometry, groundMaterial);
ground.rotation.x = -Math.PI / 2;
ground.castShadow = true;
ground.receiveShadow = true;
scene.add(ground);

// Cannon-es setup
const world = new CANNON.World();
world.gravity.set(0, -9.82, 0);

// Cube physics
const cubeShape = new CANNON.Box(new CANNON.Vec3(0.5, 0.5, 0.5));
const cubeBody = new CANNON.Body({ mass: 1 });
cubeBody.addShape(cubeShape);
cubeBody.position.set(0, 5, 0);
world.addBody(cubeBody);

// Ground physics
const groundShape = new CANNON.Plane();
const groundBody = new CANNON.Body({ mass: 0 });
groundBody.addShape(groundShape);
groundBody.quaternion.setFromEuler(-Math.PI / 2, 0, 0);
world.addBody(groundBody);

// Animation loop
function animate() {
    requestAnimationFrame(animate);

    // Update physics
    world.step(1 / 60);
    cube.position.copy(cubeBody.position);
    cube.quaternion.copy(cubeBody.quaternion);

    // Update controls
    controls.update();

    // Render scene
    renderer.render(scene, camera);
}

// Initial camera position
camera.position.set(0, 2, 5);
controls.update();

animate();

// Handle window resize
window.addEventListener('resize', () => {
    camera.aspect = window.innerWidth / window.innerHeight;
    camera.updateProjectionMatrix();
    renderer.setSize(window.innerWidth, window.innerHeight);
});