#!/bin/bash
#cat $1 | tee >(./render_animation.py) | ./play_animation.py
((sleep 1; cat $1) & id=$!) | pee ./render_animation.py ./play_animation.py

#cat $1 | tee >(./render_animation.py) >(./play_animation.py)
#cat $1 | ./play_animation.py &
#sleep 1
#cat $1 | ./render_animation.py &
#fg %1