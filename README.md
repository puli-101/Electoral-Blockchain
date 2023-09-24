# Electoral Blockchain

## Abstract
The project aims to address the challenges associated with determining the winner of an electoral process while ensuring trust, transparency, and security. It focuses on implementing cryptographic tools and a declaration system secured by asymmetric encryption. The project also involves handling a centralized declaration database, implementing a consensus mechanism, and managing a decentralized declaration base. The core concept is that citizens use pairs of keys to create and verify vote declarations, ensuring the authenticity of the voting process through encryption and decryption processes.

In this project, we address the issue of determining the winner of an electoral process. In an electoral process, each participant can declare their candidacy for the election and/or vote for a declared candidate. Electoral processes have historically raised challenging questions about trust and transparency, as elections are typically organized by the incumbent executive system, which is often a candidate for re-election and therefore suspected of interference. Additionally, the counting of votes involves poll workers, making it a time-consuming process with limited reliability since not everyone can verify that the count was conducted honestly. Furthermore, the anonymity of paper ballots means that no one can verify afterward whether their vote was counted for the correct candidate.

Another aspect to consider is the user-friendliness for voters. Specifically, a decentralized system would enable remote voting, which has long been considered a tool to combat voter abstention, which reached historic levels in the June 2021 regional and departmental elections in France (66.7% overall and up to 87% among those under 25). A bill was also submitted to the National Assembly on September 21, 2021, to allow postal voting. This bill represents a first step towards electronic voting, with the main argument being that postal voting has been in place in Germany since 1957, potentially justifying the difference in voter turnout between France and Germany (where it does not exceed 33%).

The objective of this project is to propose a framework for considering the protocols and data structures to effectively implement the process of designating the election winner while ensuring the integrity, security, and transparency of the election.

In this project, we consider the organization of an electoral process by single-member majority voting in two rounds (like here in France).

- Part 1: Implementation of cryptography tools.
- Part 2: Creation of a declaration system secured by asymmetric encryption. 
- Part 3: Handling of a centralized declaration database.
- Part 4: Implementation of a consensus mechanism.
- Part 5: Handling a decentralized declaration base

In our model, each citizen has an electoral card defined by a pair of keys:

- A secret (or private) key used to sign their vote declaration. This key should only be known to the citizen.
- A public key allowing other citizens to verify the authenticity of their declaration (signature verification). This key is also used to identify them in a vote declaration, not only when they vote but also when someone wishes to vote on their behalf.

In this exercise, we will see that signing a declaration is done by encrypting the content (using the secret key), and verifying a signature is simply done by decrypting it (using the public key).

Furthermore, in our context, a vote declaration simply involves transmitting the public key of the candidate being voted for. In an election process, it's necessary for each person to be able to produce signed vote declarations to verify their authenticity. This signature consists of an array of integers that can only be generated by the sender of the declaration (using their secret key) but can be verified by anyone (using the sender's public key). Specifically, we will use the following vote declaration protocol:

- A voter E wishes to vote for candidate C. In this case, E's vote declaration is the message "mess" obtained by converting candidate C's public key into a string representation (using the "key to str" function).

- Before publishing their declaration, voter E uses a signature function (referred to as "sign" here) to generate the signature associated with their vote declaration. This signature will take the form of an array of integers obtained by encrypting the message "mess" with voter E's secret key (using the "encrypt" function).

- The voter can then publish a secure declaration, consisting of their message "mess," the associated signature, and their public key. In this way, anyone wishing to verify the authenticity of the declaration can do so by decrypting the signature with voter E's public key: the result obtained should exactly match the message "mess."

Limitations: it is supposed that the set of candidats is already known