cmd_/home/osm/DCP/etc/JohnnyCache/jc.mod := printf '%s\n'   jc.o | awk '!x[$$0]++ { print("/home/osm/DCP/etc/JohnnyCache/"$$0) }' > /home/osm/DCP/etc/JohnnyCache/jc.mod
