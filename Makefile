SRC = 	src/base/sort.c					\
		src/base/fixed_queue.c			\
		src/os/thread.c					\
		src/os/cond_variable.c			\
		src/os/mutex.c					\
		src/os/time.c					\
		src/os/socket.c					\
		src/parallel/thread_pool.c		\
		src/parallel/job_counter.c		\
		src/parallel/parallel_for.c		\
		src/parallel/parallel_queue.c	\
		src/networking/server.c			\
		src/networking/client.c			\
		src/game/ray.c					\
		src/game/world.c				\
		src/game/player.c				\
		src/game/entity.c				\
		src/game/npc_movement.c			\
		src/game/simulate.c				\
		src/game/game_server.c			\
		src/render/render.c				\
		src/render/draw.c 				\
		src/render/assets.c				\
		src/setup.c

MAIN        = src/main.c
OUT         = bin/main

CFLAGS      = -std=c99 -MMD -MP -Werror=return-type
FLAGS       = -DDEBUG=1
INCLUDES    = -Iinclude -Isrc
LIBDIRS     = -Llib
LIBS        = -lraylib -lopengl32 -lgdi32 -lwinmm -lws2_32

OBJDIR      = obj

ALL_SRC     = $(SRC) $(MAIN)
OBJ         = $(patsubst src/%.c,$(OBJDIR)/%.o,$(ALL_SRC))
DEPS        = $(OBJ:.o=.d)

run: $(OUT)

$(OUT): $(OBJ)
	gcc $(OBJ) -o $(OUT) $(LIBDIRS) $(LIBS)

$(OBJDIR)/%.o: src/%.c
	@if not exist "$(dir $@)" mkdir "$(dir $@)"
	gcc $(CFLAGS) $(FLAGS) $(INCLUDES) -c $< -o $@

-include $(DEPS)

clean:
	rm -rf $(OBJDIR)
	rm -f $(OUT)