FROM alpine:latest
# Was originally tested using  alpine:3.13.1 
# digest @sha256:d6de6e6a9c2145e130764454005e254c91bd63657a0c0a03e8ba4d737a754a75

LABEL authors="Gary Garcia garciag@cubic.com"

RUN apk update && apk --no-cache add \
	bash \
	g++ \
	gawk

WORKDIR /home
ENTRYPOINT ["/bin/bash"]