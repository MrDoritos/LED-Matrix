#!/bin/bash
cat $1 | tee >(./play_animation.py) >(./render_animation.py)