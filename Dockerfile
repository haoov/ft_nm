FROM ubuntu:latest

RUN apt update; \
	apt install binutils -y

COPY ./ft_nm .

CMD tail -f > /dev/null
