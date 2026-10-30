#!/usr/bin/env bash

BOOK="address_book.txt"

while true; do
    echo "=============================="
    echo " ADDRESS BOOK MENU"
    echo "=============================="
    echo "1. Create Address Book"
    echo "2. View Address Book"
    echo "3. Insert a Record"
    echo "4. Delete a Record"
    echo "5. Modify a Record"
    echo "6. Exit"
    echo "=============================="

    read -r -p "Enter your choice: " choice

    case "$choice" in
        1)
            : > "$BOOK"
            echo "Address Book Created Successfully."
            ;;
        2)
            if [ -f "$BOOK" ]; then
                echo "----- Address Book -----"
                if [ -s "$BOOK" ]; then
                    cat "$BOOK"
                else
                    echo "Address book is empty."
                fi
            else
                echo "Address Book does not exist."
            fi
            ;;
        3)
            read -r -p "Enter Name: " name
            read -r -p "Enter Phone: " phone
            read -r -p "Enter Email: " email
            printf '%s|%s|%s\n' "$name" "$phone" "$email" >> "$BOOK"
            echo "Record Inserted Successfully."
            ;;
        4)
            read -r -p "Enter Name to Delete: " name
            if [ -f "$BOOK" ] && grep -Fqi "$name|" "$BOOK"; then
                grep -Fiv "$name|" "$BOOK" > temp.txt
                mv temp.txt "$BOOK"
                echo "Record Deleted Successfully."
            else
                echo "Record Not Found."
            fi
            ;;
        5)
            read -r -p "Enter Name to Modify: " name
            if [ -f "$BOOK" ] && grep -Fqi "$name|" "$BOOK"; then
                grep -Fiv "$name|" "$BOOK" > temp.txt
                mv temp.txt "$BOOK"
                read -r -p "Enter New Phone: " phone
                read -r -p "Enter New Email: " email
                printf '%s|%s|%s\n' "$name" "$phone" "$email" >> "$BOOK"
                echo "Record Modified Successfully."
            else
                echo "Record Not Found."
            fi
            ;;
        6)
            echo "Exiting..."
            exit 0
            ;;
        *)
            echo "Invalid Choice!"
            ;;
    esac

    echo
 done
