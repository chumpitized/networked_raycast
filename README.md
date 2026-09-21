# Networked Raycast
<img width="852" height="480" alt="network_cats" src="https://github.com/user-attachments/assets/8132a515-2e67-45e3-9431-d9a829dfc289" />

This project introduces a lot of low-level primitives for parallelizing floor & ceiling rendering, allowing a thread_pool of `cpu_count` workers to handle an equal share of the load. That gave the FPS a substantial boost.

In addition, I wrote a socket API for enabling entities to be controlled across the network. Right now this project acts as server and client, which both run on dedicated threads, and it's also handling all of the graphics rendering. In a traditional networked game, the server would simply receive, simulate, and confirm client requests, while clients would manage the rendering. This project does it all, for now.

On the networking side, clients submit requests to the server at an established tick rate, and the server shoves them into a lock-free parallel queue for processing. We use a parallel queue because the simulation and server pieces run on separate threads, which means we need to produce to and consume from the queue without encountering bugs.

The gif above shows 1000 cats marching back and forth across the network at 60 fps.
