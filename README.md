# Keysaver

Many people use the same password for multiple services. Needless to say, this is an unsafe approach. If one service is compromised, all others will be at risk.

Keysaver is an application designed to generate unique passwords for each service based on a single master password. Passwords are generated using cryptographically secure hashing algorithms and a CSPRNG. The following data is used for generation: the master password, the service URL, and one of nine salts chosen by the user. Salts allow for generating multiple passwords for the same service. For convenience, each salt is associated with an image in the UI.

Keysaver does not store the generated passwords but guarantees that for the same service, when entering the same master password and selecting the same salt, the same password will be generated. This means you only need to remember your master password and the image of the salt you used.

Users can customize the password generation configuration, including password length, the use of uppercase or lowercase letters, digits, and special characters. The set of special characters is also configurable. Each service can have its own password configuration.

## Technical Details

### Password Generation

The cryptographically secure pseudorandom number generator (CSPRNG) used for generating password characters is the AES-256-OFB algorithm. The key is derived from the BLAKE2s-256 hash of the master password.

To generate a service password, a SHA3-256 hash of the service URL, salted with the selected salt, is computed. The result of this hashing process serves as the initialization vector for the CSPRNG.

### Stored Data

Service information and password configuration descriptions are stored in an encrypted format. \
The following service data is saved:
- Service name
- Service URL
- Password configuration used

Data encryption is performed using the AES-256-OFB algorithm, with the SHA3-256 hash of the master password as the encryption key and the SHA-256 hash of the master password as the initialization vector.

Different hashing algorithms are used for password generation and data encryption to ensure that a compromise of the encryption key does not lead to the compromise of generated passwords.

### Third-Party Libraries
- [OpenSSL 3.4.0](https://github.com/openssl/openssl/tree/openssl-3.4.0)
- [Protocol Buffers v29.3](https://github.com/protocolbuffers/protobuf/tree/v29.3)

### Build
- TODO