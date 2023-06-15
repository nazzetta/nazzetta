git:
	git add .
	git commit -m "$m"
	git push

server:
	@printf "\n\tMy IP: "
	@ipconfig getifaddr en0
	@printf "\n"
	# python -m SimpleHTTPServer 8000	# old way of creating server
	python3 -m http.server


serverP:
	@printf "\n\tMy IP: "
	@ipconfig getifaddr en0
	@printf "\n"
	while true; do nc -l 10000 < /dev/null ; printf '\n\n\n'; done
