//Represents a single particle.
struct EmitterParticle {
    vmml::vec3f pos;
    vmml::vec3f velocity;
    vmml::vec3f color;
    float timeAlive; //The amount of time that this particle has been alive.
    float lifespan;  //The total amount of time that this particle is to live.
};