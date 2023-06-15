git:
	git add .
	git commit -m "$m"
	git push

server:
	@printf "\n\tMy IP: "
	@ipconfig getifaddr en0
	@printf "\n"
	python -m SimpleHTTPServer 8000
	