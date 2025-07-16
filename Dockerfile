# Stage 1: Build the app
FROM node:20-alpine AS builder

# Set the working directory inside the Docker container
WORKDIR /app

# Copy the package.json and package-lock.json (or yarn.lock) first to leverage Docker cache for dependencies
COPY package*.json ./

# Copy the auth file for proxy
ARG JSON_KEY_FILE
COPY ${JSON_KEY_FILE} ./storage_key.json

# Install dependencies
RUN npm install

# Copy the rest of the application source code to the container
COPY . .

# Copy the tsconfig.json for TypeScript build
COPY tsconfig.json ./

# Build the TypeScript code
RUN npm run build

# Stage 2: Run the app
FROM node:20-alpine AS runner

# Set the working directory inside the Docker container
WORKDIR /app

# Copy the build output and required files from the builder stage
COPY --from=builder /app/dist ./dist
COPY --from=builder /app/package*.json ./
COPY --from=builder /app/storage_key.json ./

# Install production dependencies only
RUN npm install --production

# Expose the application port
EXPOSE 3000

# Set environment variables (you can use dotenv in the app to load from .env)
ENV NODE_ENV=production

# Command to run the application
CMD ["node", "dist/index.js"]
