import QtQuick

QtObject {
    id: validationHelper

    // =================== VALIDATION RESULTS ===================
    function validationResult(isValid, message = "") {
        return { isValid: isValid, message: message }
    }

    // =================== STRING VALIDATIONS ===================
    function validateNotEmpty(value) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        if (value.trim() === "") {
            return validationResult(false, "This field cannot be empty")
        }
        return validationResult(true)
    }

    function validateAlphanumeric(value) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        if (!/^[a-zA-Z0-9\s\-_().,]+$/.test(value)) {
            return validationResult(false, "Only letters, numbers, spaces, and basic punctuation allowed")
        }
        return validationResult(true)
    }

    function validateComboBoxSelection(index, fieldName = "Selection") {
        if (index === -1) {
            return validationResult(false, "Please select " + fieldName)
        }
        return validationResult(true)
    }

    // ===================  VALIDATIONS ===================
    function validateEmail(value) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        var emailRegex = /^[^\s@]+@[^\s@]+\.[^\s@]+$/
        if (!emailRegex.test(value)) {
            return validationResult(false, "Invalid email format")
        }
        return validationResult(true)
    }

    function validatePhone(value) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        var digits = value.replace(/\D/g, '')
        if (digits.length < 10) {
            return validationResult(false, "Phone number must be at least 10 digits")
        }
        return validationResult(true)
    }

    function validateMinLength(value, minLength) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        if (value.length < minLength) {
            return validationResult(false, "Minimum length is " + minLength + " characters")
        }
        return validationResult(true)
    }

    function validateMaxLength(value, maxLength) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        if (value.length > maxLength) {
            return validationResult(false, "Maximum length is " + maxLength + " characters")
        }
        return validationResult(true)
    }

    // =================== NUMBER VALIDATIONS ===================
    function validatePositiveNumber(value) {
        var num = parseFloat(value)
        if (isNaN(num)) {
            return validationResult(false, "Must be a number")
        }
        if (num <= 0) {
            return validationResult(false, "Number must be positive")
        }
        return validationResult(true)
    }

    function validateInteger(value) {
        var num = parseFloat(value)
        if (isNaN(num) || !Number.isInteger(num)) {
            return validationResult(false, "Must be an integer")
        }
        return validationResult(true)
    }

    // =================== CUSTOM REGEX VALIDATION ===================
    function validateWithRegex(value, pattern, errorMessage) {
        if (typeof value !== "string") {
            return validationResult(false, "Input must be a string")
        }
        var regex = new RegExp(pattern)
        if (!regex.test(value)) {
            return validationResult(false, errorMessage)
        }
        return validationResult(true)
    }
}
