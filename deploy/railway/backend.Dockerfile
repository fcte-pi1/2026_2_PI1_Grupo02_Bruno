FROM node:22-alpine AS front
WORKDIR /front
COPY src/frontend/package.json src/frontend/package-lock.json ./
RUN npm ci
COPY src/frontend/ ./
RUN npm run build

FROM maven:3.9.9-eclipse-temurin-21 AS back
WORKDIR /workspace
COPY src/backend/pom.xml .
RUN mvn -B -q dependency:go-offline || true
COPY src/backend/src ./src
COPY --from=front /front/dist ./src/main/resources/static
RUN mvn -B -DskipTests package

FROM eclipse-temurin:21-jre
WORKDIR /app
COPY --from=back /workspace/target/ratocego-backend-1.jar app.jar
ENV JAVA_OPTS="-XX:MaxRAMPercentage=70 -XX:+UseSerialGC"
EXPOSE 8080
CMD ["sh", "-c", "exec java $JAVA_OPTS -Dserver.port=${PORT:-8080} -jar /app/app.jar"]