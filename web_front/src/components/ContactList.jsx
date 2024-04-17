import ContactListItem from "./ContactListItem";
import i from "../assets/user.png"
export default function ContactList() {
    return (
        <ContactListItem avatarUrl={i} chatName={'jack'} />
    );
}
