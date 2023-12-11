cmd_/home/osm/DCP/code/static/static.mod := printf '%s\n'   static.o | awk '!x[$$0]++ { print("/home/osm/DCP/code/static/"$$0) }' > /home/osm/DCP/code/static/static.mod
