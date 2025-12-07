#Dockerfile
FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    gcc \
    make \
	lcov \
	clang && \
	rm -rf /var/lib/apt/lists/* && \
	mkdir -p /app/src/cat /app/src/grep && chmod 777 /app/src/cat /app/src/grep && \
	groupadd --system --gid 777 appuser && \
	useradd --system --gid appuser --uid 777 appuser

COPY cat/* /app/src/cat/
COPY grep/* /app/src/grep/
COPY .clang-format /app/src/grep/

RUN chown -R appuser:appuser /app/src && \
    chmod +x /app/src/cat/*.sh /app/src/grep/*.sh 2>/dev/null || true

WORKDIR /app/src/

USER appuser

HEALTHCHECK NONE
