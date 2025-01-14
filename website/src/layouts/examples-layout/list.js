import { useState } from "react";
import { FontAwesomeIcon } from "@fortawesome/react-fontawesome";
import { withTranslation } from "next-i18next";

import cx from "classnames";
import { useRouter } from "next/router";
import Link from "next/link";

import { faEye, faEyeSlash } from "@fortawesome/free-solid-svg-icons";

import styles from "./styles.module.scss";
import Image from "../../components/image";

const Item = withTranslation("examples")(({ label, to, t }) => {
  const router = useRouter();
  const link = `/examples${to}`;
  const className = cx({
    [styles.link]: true,
    [styles.active]: link === router.pathname,
  });
  return (
    <li>
      <Link href={link} className={className}>
        {t(label)}
      </Link>
    </li>
  );
});

function MenuButton({ onClick, visible }) {
  const icon = visible ? faEye : faEyeSlash;
  return (
    <button type="button" onClick={onClick} className={styles.button}>
      <FontAwesomeIcon icon={icon} />
    </button>
  );
}

const MenuGroup = withTranslation("examples")(({ label, children, t }) => {
  const [visible, setVisible] = useState(true);
  const onToggle = () => setVisible(!visible);
  return (
    <div className={styles.group}>
      <div className={styles["group-header"]}>
        <strong>{t(label)}</strong>
        <MenuButton onClick={onToggle} visible={visible} />
      </div>
      {visible && <ul className={styles.items}>{children}</ul>}
    </div>
  );
});

const Header = withTranslation("examples")(({ t }) => {
  const router = useRouter();
  const onClick = () => {
    router.push("/");
  };
  return (
    <>
      <header className={styles.header} tabIndex={0} onClick={onClick}>
        <Image src="/img/logo.svg" className={styles.logo} />
        <div>
          <h1>
            Ca<span className={styles.hightlight}>rb</span>uncle
          </h1>
          <h2>{t("Examples")}</h2>
        </div>
      </header>
      <hr />
    </>
  );
});

function ExampleList() {
  return (
    <nav className={styles.menu}>
      <Header />
      <MenuGroup label="Core examples">
        <Item to="/core/hello_world" label="Hello World" />
      </MenuGroup>
      <MenuGroup label="Graphics">
        <Item to="/graphics/image_generation" label="Image Generation" />
      </MenuGroup>
      <MenuGroup label="Demos">
        <Item to="/demos/snake" label="Snake" />
      </MenuGroup>
    </nav>
  );
}

export default ExampleList;
