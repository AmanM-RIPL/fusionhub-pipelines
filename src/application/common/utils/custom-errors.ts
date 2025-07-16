export class CustomError extends Error {
  public readonly statusCode: number;
  public readonly code: string;

  constructor(message: string, statusCode = 500, code = 'INTERNAL_SERVER_ERROR') {
    super(message);
    this.name = this.constructor.name; // Keep the class name as the error name
    this.statusCode = statusCode;
    this.code = code;
    Error.captureStackTrace(this, this.constructor); // Captures stack trace
  }
}

export class ValidationError extends CustomError {
  public readonly details: string[];

  constructor(message = 'Validation failed', details = []) {
    super(message, 400, 'VALIDATION_ERROR');
    this.details = details; // Optional: include specific validation errors
  }
}

export class NotFoundError extends CustomError {
  constructor(message = 'Resource not found') {
    super(message, 404, 'NOT_FOUND');
  }
}

export class UnauthorizedError extends CustomError {
  constructor(message = 'Unauthorized access') {
    super(message, 401, 'UNAUTHORIZED');
  }
}

export class ForbiddenError extends CustomError {
  constructor(message = 'Forbidden access') {
    super(message, 403, 'FORBIDDEN');
  }
}

export class InternalServerError extends CustomError {
  public readonly originalError: Error | null;

  constructor(message = 'An unexpected error occurred.', originalError = null) {
    super(message, 500, 'INTERNAL_SERVER_ERROR');
    this.originalError = originalError; // Store the original error for logging
  }
}